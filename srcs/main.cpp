#include "../headers/irc.h"

bool server_statut = true;

struct pollfd	*new_tab( struct pollfd *fds, int socket_nbr ) {
	struct pollfd	*new_tab = new pollfd[socket_nbr + 1];
	int			count = 0;

	while ( count < socket_nbr ) {
		new_tab[count] = fds[count];
		count++;
	}
	delete [] fds;
	return new_tab;
}

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		server_statut = false;
	}
}

int	create_server_link( char *port ) {

	int					server_socket;
	struct sockaddr_in	serverAddress;
	int					ret;

	// signal(SIGQUIT, SIG_IGN);  //que faire 
	signal(SIGINT, sig_handler);

	/* Création de la socket d'ecoute du serveur */
	server_socket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( server_socket == -1 ) {
		std::cerr << "Failed to create socket: " << strerror( errno ) << std::endl;
		return -1;
	}

    /* Préparation de l'adresse IP et du port du serveur */
	std::memset( &serverAddress, 0, sizeof( serverAddress ) );
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
	serverAddress.sin_port = htons( atoi( port ) );

    /* Attribution de l'adresse IP et du port à la socket */
	ret = bind( server_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress) );
	if ( ret == -1 ) {
		std::cerr << "Failed to bind socket: " << strerror( errno ) << std::endl;
		return -1;
	}
	struct sockaddr_in	name;
	socklen_t			len = sizeof( name );
	getsockname( server_socket, (struct sockaddr *)&name, &len );
	// std::cout << "sin_family = " << name.sin_family << "\nsin_port = " << ntohs(name.sin_port)
	// 		<< "\nsin_addr.s_addr = " << inet_ntoa(name.sin_addr) << std::endl;

    /* Attente de connexions entrantes */
	ret = listen( server_socket, 10 );
	if ( ret == -1 ) {
		std::cerr << "Error while executing listen(): " << strerror( errno ) << std::endl;
		return -1;
	}
	return server_socket;
}

void	end_close( struct pollfd *fds, int socket_nbr ) {
	if (fds == NULL)
		return ;
	for ( int i = 0; i < socket_nbr && &fds[i]; i++ ) {
		if (fds[i].fd >= 0) {
			close( fds[i].fd );
			fds[i].fd = -1;
		}
	}
	delete [] fds;
}


int	incoming_connections(struct pollfd **fds, t_context &context)
{
	struct sockaddr_in		clientAddress;
	socklen_t 				clientAddressLength = sizeof(clientAddress);

	if ((*fds)[0].revents & POLLIN)
	{
		/* Acceptation de la première connexion entrante */
		*fds = new_tab( *fds, context.socket_nbr[0]);
		(*fds)[context.socket_nbr[0]].events = POLLIN;
		(*fds)[context.socket_nbr[0]].revents = 0;
        (*fds)[context.socket_nbr[0]].fd = accept((*fds)[0].fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if ((*fds)[context.socket_nbr[0]].fd == -1)
		{
			std::cerr << "Error while executing accept() : " << strerror(errno) << std::endl;
			return 1;
		}
		Client new_client((*fds)[context.socket_nbr[0]].fd, clientAddress);
		context.clients.insert( std::map<int, Client>::value_type( (*fds)[context.socket_nbr[0]].fd, new_client ) );
		(context.socket_nbr[0])++;
	
		// std::cout << "Connexion acceptée depuis " << inet_ntoa(new_client.getIp().sin_addr) << ":" << ntohs(new_client.getIp().sin_port) << std::endl;
		// std::cout << "Connexion acceptée depuis " << new_client.getHost() << std::endl;
		// std::cout << "Son fd est : " <<  new_client.getFd() << std::endl;
	}
	return 0;
}

void	t_func_initialize(t_func_ptr *funcTab)
{
	funcTab[0].name = "INVITE";
	funcTab[0].ptr = &ft_invite;
	funcTab[1].name = "JOIN";
	funcTab[1].ptr = &ft_join;
	funcTab[2].name = "KICK";
	funcTab[2].ptr = &ft_kick;
	funcTab[3].name = "KILL";
	funcTab[3].ptr = &ft_kill;
	funcTab[4].name = "LIST";
	funcTab[4].ptr = &ft_list;
	funcTab[5].name = "MODE";
	funcTab[5].ptr = &ft_mode;
	funcTab[6].name = "NAMES";
	funcTab[6].ptr = &ft_names;
	funcTab[7].name = "NICK";
	funcTab[7].ptr = &ft_nick;
	funcTab[8].name = "OPER";
	funcTab[8].ptr = &ft_oper;
	funcTab[9].name = "PART";
	funcTab[9].ptr = &ft_part;
	funcTab[10].name = "PASS";
	funcTab[10].ptr = &ft_pass;
	funcTab[11].name = "PING";
	funcTab[11].ptr = &ft_ping;
	funcTab[12].name = "PRIVMSG";
	funcTab[12].ptr = &ft_privmsg;
	funcTab[13].name = "privmsg";
	funcTab[13].ptr = &ft_privmsg;
	funcTab[14].name = "QUIT";
	funcTab[14].ptr = &ft_quit;
	funcTab[15].name = "TOPIC";
	funcTab[15].ptr = &ft_topic;
	funcTab[16].name = "USER";
	funcTab[16].ptr = &ft_user;
	funcTab[17].name = "WHO";
	funcTab[17].ptr = &ft_who;
}

int	client_request( struct pollfd **fds, Client *tmp, std::string ref, int i, t_context *context)
{
		// Recherche de la fonction correspondante et appel si trouvée
		t_func_ptr funcTab[18];
		t_func_initialize(funcTab);
		std::string cmd;
		std::vector<std::string> args;
		int			j;

		args = ft_split( ref, " " );
		cmd = args[0];
		args.erase(args.begin());
		for(j = 0; j < 18; j++)				// si la commande fait partie des operateurs
		{
			if (cmd == funcTab[j].name)
			{
				if(cmd == "NICK" || cmd == "PASS" || cmd == "USER" || tmp->canConnect() != 0)
					(*(funcTab[j].ptr))(context, tmp, *fds, i, args);
				else
					isAuthorized(*tmp, cmd);
				break ;
			}
		}
		if (j == 18 && cmd != "CAP")
		{
			std::string	reply = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), cmd, (std::string)"Unknown command");
			send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		}
		return 0;
}

void	check_clients_sockets(struct pollfd **fds, char *buffer, t_context *context)
{
	int 		ret;
	int			pos;
	std::string cmd;

	/* Verification de demande de communication */
	for ( int i = 1; i < context->socket_nbr[0]; i++ )
	{
		if ((*fds)[i].fd >= 0 && (*fds)[i].revents & POLLIN)
		{
			bzero( buffer, 1024 );
			ret = recv((*fds)[i].fd, buffer, sizeof(buffer), 0 );
			if (ret == 0 || (ret == -1 && errno == 9))
			{
				std::map<int, Client>::iterator it = context->clients.find((*fds)[i].fd);
				if (it != context->clients.end())
				{
					std::string reply = RPL_QUIT(it->second.getNickname(), it->second.getUsername(), it->second.getHost(), "Leaving");
					it->second.leaveAllChannels(context, reply);
					context->clients.erase(it);
				}
				if ((*fds)[i].fd) {
					close ((*fds)[i].fd);
					(*fds)[i].fd = -1;
				}
			}
			else if (ret == -1 && errno != 9)
			{
				std::cerr << "Error while receiving the message with recv() : " << strerror(errno) << std::endl;
				return ;
			}
			else
			{
				Client *tmp = &context->clients.find((*fds)[i].fd)->second;
				(*tmp).add_buff(buffer);
				const std::string &ref = (*tmp).getBuffer();
				size_t	position = ref.rfind( "\r\n" );
				if ( position == ref.size() - 2 )
				{
					ret = ref.find("\r\n");
					pos = 0;
					while (ret != -1)
					{
						cmd = ref.substr(pos, ret - pos);
						// std::cout << "Received : " << cmd << std::endl;
						client_request(fds, tmp, cmd, i, context);
						if (context->clients.find((*fds)[i].fd) == context->clients.end() )
							break;
						
						pos = ret + 2;
						ret = ref.find("\r\n", ret + 2);
					}
					if ( ret == -1 )
						(*tmp).clear();
				}
			}
		}
		bzero(buffer, strlen(buffer));
	}
}

int	initialize_context(t_context &context, int *socket_nbr, std::string password)
{
	std::map<std::string, Channel>	channels;
	std::ifstream 					confFile;
	std::string						line;
	std::size_t 					found;
	
	confFile.open("./.IRCd-config"); // Ouvrir le fichier en lecture
    if (!confFile) {
        std::cout << "Failed to open conf file : " << strerror(errno) << std::endl;
		confFile.close();
        return 1;
    }
    while (std::getline(confFile, line)) {
		found = line.find("operator_host =");
        if (found != std::string::npos && line.size() > sizeof("operator_host =") -1 )
            context.op_host  = line.substr(found + sizeof("operator_host ="));
        found = line.find("operator_name =");
        if (found != std::string::npos && line.size() > sizeof("operator_name =") -1)
            context.op_name  = line.substr(found + sizeof("operator_name ="));
        found = line.find("operator_password =");
       if (found != std::string::npos && line.size() > sizeof("operator_password ="))
            context.op_password = line.substr(found + sizeof("operator_password =") - 1);
    }
	if (context.op_host.empty()) {
		std::cout << "Error in IRCd-config : No operator host provided." << std::endl;
		 confFile.close();
		return 1;
	}
	if ((context.op_name.empty() && !context.op_password.empty()) || (!context.op_name.empty() && context.op_password.empty()) ) {  // pour ne pas qu'il y ait de name sans mdp ou inversement
		std::cout << "Error in IRCd-config : There should be a name and a password for the operator." << std::endl;
		confFile.close();
		return 1;
	}
    confFile.close();
	context.channels = channels;
	context.socket_nbr[0] = *socket_nbr;
	context.password = password;
	return 0;
}

struct pollfd	*check_communication( struct pollfd *fds, int *socket_nbr, std::string password)
{
	int						ret;
	char					buffer[1024];			// limite d'une reception ???

	std::string			nick;
	std::string			username;
	std::string			response;
	std::string			message;
	t_context			context;

	bzero(buffer, 1024);
	if (initialize_context(context, socket_nbr, password) != 0) {
		delete [] fds;
		return NULL;
	}
	while (server_statut == true)
	{
		ret = poll(fds, context.socket_nbr[0], -1);
        if (ret < 0)
		{
            std::cerr << "Error while executing poll() : " << strerror(errno) << std::endl;
			delete [] fds;
            return NULL;
        }
		incoming_connections(&fds, context);
		check_clients_sockets(&fds, buffer, &context);
	}
	*socket_nbr = context.socket_nbr[0];
	return fds;
}

int	parse_number( const char *number ) {
	int count = 0;
	for ( ; isdigit(number[count]) != 0; count++ );
	if ( number[count] != '\0' )
		return -1;
	return 0;
}	

void	shutdown_server(struct pollfd *fds, int *socket_nbr)
{
	end_close( fds, *socket_nbr );
	std::cout << "Goodbye !" << std::endl;
}

int	main( int argc, char **argv ) {
	struct pollfd		*fds = new pollfd[1];
	int					socket_nbr[1];

	if ( argc != 3 ) {
		std::cout << "Error number of arguments" << std::endl;
		delete[] fds;
		return 1;
	}
	if ( parse_number( argv[1] ) == -1 ) {
		std::cout << "Error port" << std::endl;
		delete[] fds;
		return 1;
	}
	if ( argv[2][0] == '\0' ) {
		std::cout << "Empty pass" << std::endl;
		delete[] fds;
		return 1;
	}

	fds[0].fd = create_server_link( argv[1] );
	if ( fds[0].fd == -1 )
	{
		delete [] fds;
		return -1;
	}
	fds[0].events = POLLIN;
	
	*socket_nbr = 1;
	fds = check_communication( fds, socket_nbr, argv[2]);
	if ( fds == NULL )
	{
		shutdown_server(fds, socket_nbr);
		return -1;
	}

	end_close( fds, *socket_nbr );
	std::cout << "Goodbye !" << std::endl;

    return (0);
}