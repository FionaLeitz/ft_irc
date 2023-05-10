#include "headers/irc.h"
#include "headers/Client.hpp"

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
// int	create_server_link( void ) {

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
	serverAddress.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
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



struct pollfd	*check_communication( struct pollfd *fds, int *socket_nbr ) {
	int						ret;
	struct sockaddr_in		clientAddress;
	char					buffer[1024];			// limite d'une reception ???
	socklen_t 				clientAddressLength = sizeof(clientAddress);
	std::map<int, Client>	clients;

	int					pos;
	std::string			nick;
	std::string			username;
	std::string			response;
	std::string			message;

	while ( 1 )
	{
		ret = poll(fds, *socket_nbr, -1);
        if (ret < 0)
		{
            std::cerr << "Error while executing poll() : " << strerror(errno) << std::endl;
            return NULL;
        }
        if (fds[0].revents & POLLIN)
		{
			/* Acceptation de la première connexion entrante */
			// Client	new_client;
			fds = new_tab( fds, *socket_nbr );
			fds[*socket_nbr].events = POLLIN;
            fds[*socket_nbr].fd = accept(fds[0].fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
            if (fds[*socket_nbr].fd == -1)
			{
				std::cerr << "Error while executing accept() : " << strerror(errno) << std::endl;
				return NULL;
			}
			// new_client.fd = fds[*socket_nbr].fd;
			Client new_client(fds[*socket_nbr].fd);
			clients.insert( std::map<int, Client>::value_type( fds[*socket_nbr].fd, new_client ) );
			(*socket_nbr)++;
            std::cout << "Connexion acceptée depuis " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
			std::cout << "Son fd est : " <<  new_client.getFd() << std::endl;
		}
		for ( int i = 1; i < *socket_nbr; i++ ) {
			/* Verification de demande de communication */
			if (fds[i].revents & POLLIN)
			{
				bzero( buffer, 1024 );
				ret = recv( fds[i].fd, buffer, sizeof(buffer), 0 );
				if (ret == -1)
				{
					std::cerr << "Error while receiving the message with recv() : " << strerror(errno) << std::endl;
					end_close( fds, *socket_nbr );
					return NULL;
				}
				else if (ret == 0)
				{
					std::cout << "Client has left the chat" << std::endl;
					close (fds[i].fd);
				}
				else
				{
					std::cout << "Successfully received a message of size " << ret << " from client ! "<< std::endl;
					clients.find(fds[i].fd)->second.add_buff( buffer );
					const std::string &	ref = clients.find( fds[i].fd )->second.getBuffer();
					// ref.insert( ref.size(), buffer );
					size_t	position = ref.rfind( "\r\n" );
					if ( position == ref.size() - 2 ) {
						std::cout << "Message reçu : " << ref << std::endl;
						if(ref.find("USER") != std::string::npos)
						{
							// #IRC connection handshake consists of sending NICK and USER messages. All IRC messages must end in \r\n
							ret = ref.find("NICK") + 5;
							nick = ref.substr(ret, ref.find("USER") - 2 - ret);
							ret = ref.find("USER") + 5;
							username = ref.substr(ret, ref.find(" ", ret + 5) - ret);
							response = RPL_WELCOME(nick, username);
							std::cout << "\tnickname = " << nick << "\n\tusername = " << username << std::endl;
							clients.find(fds[i].fd)->second.setNickname(nick);
							clients.find(fds[i].fd)->second.setUsername(username);
							std::cout << "DANS LA CLASSE CLIENT" << "\tnickname = " << clients.find(fds[i].fd)->second.getNickname() << "\n\tusername = " << clients.find(fds[i].fd)->second.getUsername() << std::endl;
							send(fds[i].fd, response.c_str(), response.length(), 0);
							//jsp si utile ou pas ?
						}
						if (ref.find("coucou") != std::string::npos)
						{
							std::cout << "!!!!!!!!!" << std::endl;
							std::cout << clients.find(fds[i].fd)->second.getNickname() << " veut rejoindre un channel" << std::endl;
							response = RPL_JOIN(clients.find(fds[i].fd)->second.getNickname(), clients.find(fds[i].fd)->second.getUsername(), "#joli_channel");
							// std::cout << "###########" << std::endl << response << std::endl << RPL_JOIN(nick, username, "#joli_channel") << std::endl << std::endl;
							send(fds[i].fd, response.c_str(), response.length(), 0);
						}
						else if (ref.find("MODE") != std::string::npos)
						{
							response = ":server 324 " + clients.find(fds[i].fd)->second.getNickname() + " #joli_channel +nt \r\n";
							send(fds[i].fd, response.c_str(), response.length(), 0);
						}
						else if (ref.find("WHO") != std::string::npos)
						{
							response = ":server 352 " + clients.find(fds[i].fd)->second.getNickname() + " #joli_channel " + clients.find(fds[i].fd)->second.getNickname() + " user host server " + clients.find(fds[i].fd)->second.getNickname() + " H :0 " + clients.find(fds[i].fd)->second.getNickname() + "\r\n";
							send(fds[i].fd, response.c_str(), response.length(), 0);
						}
						else if (ref.find("PRIVMSG") != std::string::npos)
						{
							ret = ref.find("#joli_channel :") + 15;
							message = ref.substr(ret, ref.find("\r\n") - ret);
							std::cout << "message = " << message << std::endl;
							// response = RPL_MSG(clients.find(fds[i].fd)->second.getNickname(), clients.find(fds[i].fd)->second.getUsername(), "#help", message);
							response = ":" + clients.find(fds[i].fd)->second.getNickname() + " PRIVMSG #joli_channel :" + message +"\r\n";
							send(fds[i + 1].fd, response.c_str(), response.length(), 0);
						}
						ret = ref.find("\r\n");
						pos = 0;
						
						while (ret != -1)
						{
							std::cout << "Commande recue (ref[" << pos << "] -- ref[" << ret - 1 << "]) : " 
							<< ref.substr(pos, ret - pos) << std::endl;
							pos = ret + 2;
							ret = ref.find("\r\n", ret + 2);
						}
						clients.find( fds[i].fd )->second.clear();
					}
				}
			}
		}
	}
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

	// (void)argc;
	// (void)argv;
	if ( argc != 2 ) {
		std::cout << "Error number of arguments" << std::endl;
		return 1;
	}
	if ( parse_port( argv[1] ) == -1 ) {
		std::cout << "Error port" << std::endl;
		return 1;
	}

	fds[0].fd = create_server_link( argv[1] );
	// fds[0].fd = create_server_link();
	if ( fds[0].fd == -1 )
		return -1;
	fds[0].events = POLLIN;
	
	*socket_nbr = 1;
	fds = check_communication( fds, socket_nbr );
	if ( fds == NULL )
		return -1;

	end_close( fds, *socket_nbr );

    return (0);
}