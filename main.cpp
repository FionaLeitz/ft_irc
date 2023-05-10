#include "headers/irc.h"

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
	int						end = 0;
	std::map<int, t_client>	clients;

	// while ( end < 10 )
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
			t_client	new_client;
			fds = new_tab( fds, *socket_nbr );
			fds[*socket_nbr].events = POLLIN;
            fds[*socket_nbr].fd = accept(fds[0].fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
            if (fds[*socket_nbr].fd == -1)
			{
				std::cerr << "Error while executing accept() : " << strerror(errno) << std::endl;
				return NULL;
			}
			new_client.fd = fds[*socket_nbr].fd;
			clients.insert( std::map<int, t_client>::value_type( fds[*socket_nbr].fd, new_client ) );
			(*socket_nbr)++;
            std::cout << "Connexion acceptée depuis " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
		}
		for ( int i = 1; i < *socket_nbr; i++ ) {
			/* Verification de demande de communication */
			if (fds[i].revents & POLLIN)
			{
				bzero( buffer, 1024 );
				ret = recv( fds[i].fd, &buffer[end], sizeof(buffer), 0 );
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
					clients.find( fds[i].fd )->second.buffer.insert( clients.find( fds[i].fd )->second.buffer.size(), buffer );
					size_t	position = clients.find( fds[i].fd )->second.buffer.rfind( "\r\n" );
					if ( position == clients.find( fds[i].fd )->second.buffer.size() - 2 ) {
						std::cout << "Message reçu : " << clients.find( fds[i].fd )->second.buffer << std::endl;
						clients.find( fds[i].fd )->second.buffer.clear();
					}
				}
			}
		}
	}
	return fds;
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