#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <vector>

int	main( int argc, char **argv) {
	if ( argc != 2 ) {
		std::cout << "ERROR number of arguments" << std::endl;
		return 1;
	}

	int	check;
	int	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) {
		std::cout << "ERROR socket" << std::endl;
		return 2;
	}

	struct sockaddr_in	my_addr, their_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons( atoi(argv[1]) );
	my_addr.sin_addr.s_addr = INADDR_ANY;
	// my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero( &(my_addr.sin_zero), 8);

	check = bind( sockfd, ( struct sockaddr *)&my_addr, sizeof( struct sockaddr ) );
	if ( check == -1 ) {
		std::cout << "ERROR bind" << std::endl;
		return 3;
	}
	// int	new_sockfd[2];
	std::vector<int>	new_sockfd;
	int	count = 0;
	int	sin_size = sizeof( struct sockaddr_in);
	char	msg[28] = "Bien le bonjour etranger !\n";
	int	len = strlen( msg );
	int	bytes_sent;
	char	buff[500];
	while ( count < 2 ) {
		check = listen( sockfd, 10 );
		if ( check == -1 ) {
			std::cout << "ERROR listen" << std::endl;
			return 4;
		}

		new_sockfd.push_back( accept( sockfd, ( struct sockaddr *)&their_addr, ( socklen_t *)&sin_size ) );
		if ( new_sockfd[count] == -1 ) {
			std::cout << "ERROR accept" << std::endl;
			return 5;
		}
		else {
			bytes_sent = send( new_sockfd[count], msg, len, 0 );
			if ( bytes_sent == -1 ) {
				std::cout << "ERROR sent" << std::endl;
				return 6;
			}
		}

		count++;
	}
	check = recv( new_sockfd[0], buff, 500, 0);
	if ( check == -1 ) {
		std::cout << "ERROR recv" << std::endl;
		return 7;
	}
	else {
		bytes_sent = send( new_sockfd[1], buff, strlen( buff ), 0 );
		if ( bytes_sent == -1 ) {
			std::cout << "ERROR sent 2" << std::endl;
			return 8;
		}
	}



	return 0;
}
