#include "../headers/irc.h"

/*
struct sockaddr_in {
	short	sin_family;		// famille d'adresses : AF_INET   
	u_short	sin_port;		// port dans l'ordre d'octets du réseau
	struct	in_addr sin_addr; // adresse Internet   
	char	sin_zero[8];
};

// Adresse Internet
struct in_addr {
    uint32_t       s_addr;     // Adresse dans l'ordre d'octets réseau 
};

struct sockaddr {
	u_short sa_family;
	char	sa_data[14];
};
*/

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

int	create_server_link( char *port ) {

	int					server_socket;
	struct sockaddr_in	serverAddress;
	int					ret;

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
	// serverAddress.sin_port = htons( 3630 );
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
	std::cout << "sin_family = " << name.sin_family << "\nsin_port = " << ntohs(name.sin_port)
			<< "\nsin_addr.s_addr = " << inet_ntoa(name.sin_addr) << std::endl;

    /* Attente de connexions entrantes */
	ret = listen( server_socket, 10 );
	if ( ret == -1 ) {
		std::cerr << "Error while executing listen(): " << strerror( errno ) << std::endl;
		return -1;
	}

	return server_socket;
}

void	end_close( struct pollfd *fds, int socket_nbr ) {
	for ( int i = 0; i < socket_nbr; i++ )
		close( fds[socket_nbr].fd );
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
        // std::cout << "Connexion acceptée depuis " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
		std::cout << "Connexion acceptée depuis " << inet_ntoa(new_client.getIp().sin_addr) << ":" << ntohs(new_client.getIp().sin_port) << std::endl;
		std::cout << "Son fd est : " <<  new_client.getFd() << std::endl;
	}
	return 0;
}

void	t_func_initialize(t_func_ptr *funcTab)
{
	funcTab[0].name = "USER";
	funcTab[0].ptr = &ft_user;
	funcTab[1].name = "JOIN";
	funcTab[1].ptr = &ft_join;
	funcTab[2].name = "MODE";
	funcTab[2].ptr = &ft_mode;
	funcTab[3].name = "WHO";
	funcTab[3].ptr = &ft_who;
	funcTab[4].name = "PRIVMSG";
	funcTab[4].ptr = &ft_privmsg;
	funcTab[5].name = "NICK";
	funcTab[5].ptr = &ft_nick;
	funcTab[6].name = "PASS";
	funcTab[6].ptr = &ft_pass;
	funcTab[7].name = "QUIT";
	funcTab[7].ptr = &ft_quit;

}

void	ft_handshake(Client *tmp, struct pollfd *fds, int i)
{
	int			ret;
	std::string	nick;
	std::string	username;
	std::string	response;
	const std::string &	ref = (*tmp).getBuffer();

	// #IRC connection handshake consists of sending NICK and USER messages. All IRC messages must end in \r\n
	
	ret = ref.find("NICK") + 5;
	nick = ref.substr(ret, ref.find("USER") - 2 - ret);
	ret = ref.find("USER") + 5;
	username = ref.substr(ret, ref.find(" ", ret) - ret);
	response = RPL_WELCOME(nick, username);
	std::cout << "\tnickname = " << nick << "\n\tusername = " << username << std::endl;
	(*tmp).setNickname(nick);
	(*tmp).setUsername(username);
	std::cout << "DANS LA CLASSE CLIENT" << "\tnickname = " << (*tmp).getNickname() << "\n\tusername = " << (*tmp).getUsername() << std::endl;
	send(fds[i].fd, response.c_str(), response.length(), 0);
	//jsp si utile ou pas ?
}



int	client_request( struct pollfd **fds, Client *tmp, std::string ref, int i, t_context *context)
{
		std::cout << "Message reçu : " << ref << std::endl;

		// Recherche de la fonction correspondante et appel si trouvée
		t_func_ptr funcTab[8];
		t_func_initialize(funcTab);
		std::istringstream iss(ref);
		std::string cmd;
		std::string args[2];
		int			j;
		// int			ret;
		// int			pos;

		iss >> cmd >> args[0] >> args[1];  // on decoupe la string en 3 parties : cmd, args[0] et args[1];
		for(j = 0; j < 8; j++)				// si la commande fait partie des operateurs
		{
			if (cmd == funcTab[j].name)
			{
				(*(funcTab[j].ptr))(context, tmp, *fds, i, args);
				break ;
			}
		}
		if (j == 8)
		{
			std::cout << "Action non reconnue : " << ref << std::endl;
		}
		/*
		autre methode :
						enum Action { USER, JOIN, MODE, WHO, PRIVMSG, NONE};
						Action				action;
						if (action >= 0 && action < sizeof(funcTab) / sizeof(funcTab[0]))
						{
							funcTab[action](tmp, fds, i);
						}
						else if (ref.find("CAPS") != std::string::npos)
							ft_handshake(tmp, fds, i);
						else 
						{
							std::cout << "Action non reconnue : " << ref << std::endl;
						}
						*/
		// ret = ref.find("\r\n");
		// pos = 0;
		// while (ret != -1)
		// {
		// 	std::cout << "Commande recue (ref[" << pos << "] -- ref[" << ret - 1 << "]) : " 
		// 	<< ref.substr(pos, ret - pos) << std::endl;
		// 	pos = ret + 2;
		// 	ret = ref.find("\r\n", ret + 2);
		// 	(*tmp).clear();
		// }
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
		if ((*fds)[i].revents & POLLIN)
		{
			bzero( buffer, 1024 );
			ret = recv((*fds)[i].fd, buffer, sizeof(buffer), 0 );
			if (ret == -1)
			{
				std::cerr << "Error while receiving the message with recv() : " << strerror(errno) << std::endl;
				end_close(*fds, context->socket_nbr[0] );
				return ;
			}
			else if (ret == 0)
			{
				std::cout << "Client has left the chat" << std::endl;
				context->socket_nbr[0]--;
				close ((*fds)[i].fd);
			}
			else
			{
				std::cout << "Successfully received a message of size " << ret << " from client ! "<< std::endl;
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
						std::cout << "Commande recue (ref[" << pos << "] -- ref[" << ret - 1 << "]) : " 
						<< cmd << std::endl;
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

	}
}

struct pollfd	*check_communication( struct pollfd *fds, int *socket_nbr, int password)
{
	int						ret;
	// struct sockaddr_in		clientAddress;
	char					buffer[1024];			// limite d'une reception ???
	// socklen_t 				clientAddressLength = sizeof(clientAddress);
	t_context				context;
	// std::map<int, Client>	clients;
	std::map<std::string, Channel>	channels;

	std::string			nick;
	std::string			username;
	std::string			response;
	std::string			message;

	t_func_ptr funcTab[8];
	t_func_initialize(funcTab);
	context.password = password;
	context.channels = channels;
	context.socket_nbr[0] = *socket_nbr;
	// void (*funcTab[])(Client *tmp, struct pollfd *fds, int i) = { ft_user, ft_join, ft_mode, ft_who, ft_privmsg };

	while (1)
	{
		ret = poll(fds, context.socket_nbr[0], -1);
        if (ret < 0)
		{
            std::cerr << "Error while executing poll() : " << strerror(errno) << std::endl;
            return NULL;
        }
		incoming_connections(&fds, context);
		check_clients_sockets(&fds, buffer, &context);
	}
	*socket_nbr = context.socket_nbr[0];
	return fds;
}

int	parse_port( char *port ) {
	int count = 0;
	for ( ; isdigit(port[count]) != 0; count++ );
	if ( port[count] != '\0' )
		return -1;
	return 0;
}	

int	main( int argc, char **argv ) {
	struct pollfd		*fds = new pollfd[1];
	int					socket_nbr[1];

	if ( argc != 3 ) {
		std::cout << "Error number of arguments" << std::endl;
		return 1;
	}
	if ( parse_port( argv[1] ) == -1 ) {
		std::cout << "Error port" << std::endl;
		return 1;
	}

	fds[0].fd = create_server_link( argv[1] );
	if ( fds[0].fd == -1 )
		return -1;
	fds[0].events = POLLIN;
	
	*socket_nbr = 1;
	fds = check_communication( fds, socket_nbr, atoi(argv[2]));
	if ( fds == NULL )
		return -1;

	end_close( fds, *socket_nbr );

    return (0);
}