
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <poll.h>
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

int main() {
	int					serverSocket;
	int					clientSocket;
	struct sockaddr_in	serverAddress;
	struct sockaddr_in clientAddress;
	int					ret;
	struct pollfd		fds[2];
	char				buffer[256];
	std::string			buff;
	int					pos;

	/* Création de la socket d'ecoute du serveur */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
	{
        std::cerr << "Failed to create socket : " << errno << std::endl;
        return (1);
    }

    /* Préparation de l'adresse IP et du port du serveur */

    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; // toujours cette valeur mais jsp pourquoi
    serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // conversion depuis l'ordre des octets de l'hôte vers celui du réseau (pour un uint32)
	//  Lorsqu'on indique INADDR_ANY lors de l'attachement, la socket sera affectée à toutes les interfaces locales.
	// INADDR_LOOPBACK : localhost
	// autre adresse (je crois):
	// in_addr_t addr = inet_addr("172.217.160.99");
	// serverAddress.sin_addr.s_addr = addr;
    serverAddress.sin_port = htons(3630);  // htons : conversion depuis l'ordre des octets de l'hôte vers celui du réseau (pour un uint16)
	//port du pere noel

    /* Attribution de l'adresse IP et du port à la socket */

    ret = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (ret == -1)
	{
        std::cerr << "Failed to bind socket : " << strerror(errno) << std::endl;
        return (1);
    }
	else
	{
		struct sockaddr_in name;
		socklen_t len = sizeof(name);
		getsockname(serverSocket, (struct sockaddr *)&name, &len);
		std::cout << "sin_family = " << name.sin_family << "\nsin_port = " << ntohs(name.sin_port)
		<< "\nsin_addr.s_addr = " << inet_ntoa(name.sin_addr) << std::endl;
	}

    /* Attente de connexions entrantes */

    ret = listen(serverSocket, 1);
    if (ret == -1) {
        std::cerr << "Error while executing listen() : " << strerror(errno) << std::endl;
        return 1;
    }

	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	while (1)
	{
		ret = poll(fds, 2, -1);
        if (ret < 0)
		{
            std::cerr << "Error while executing poll() : " << strerror(errno) << std::endl;
            return (1);
        }
        if (fds[0].revents & POLLIN)
		{
			 /* Acceptation de la première connexion entrante */
            socklen_t clientAddressLength = sizeof(clientAddress);
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
            if (clientSocket == -1) //ou < 0 ?
			{
				std::cerr << "Error while executing accept() : " << strerror(errno) << std::endl;
				return(1);
			}
            std::cout << "Connexion acceptée depuis " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
			fds[1].fd = clientSocket;
			fds[1].events = POLLIN;
			//  send(clientSocket, ":localhost 001 cmeston :Welcome to the test IRC server cmeston\r\n", sizeof(":localhost 001 cmeston :Welcome to the test IRC server cmeston\r\n"), 0);
		}
		if (fds[1].revents & POLLIN)
		{
			buff.clear();
			bzero(buffer, 256);
			ret = recv(clientSocket, buffer, sizeof(buffer), 0);
			buff.insert(0, buffer);
			if (ret == -1)
			{
				std::cerr << "Error while receiving the message with recv() : " << strerror(errno) << std::endl;
				return(1);
			}
			else if (ret == 0)
			{
				std::cout << "Client has left the chat" << std::endl;
				close (clientSocket);
				break ;
			}
			else
			{
				std::cout << "Successfully received a message of size " << ret << " from client ! "<< std::endl;
				std::cout << "Message reçu : " << buff << std::endl;
				// if (strncmp(buffer, "coucou", 6) == 0)
				// {
				// 	std::cout << "!!!!!!!!!" << std::endl;
				// 	send(clientSocket, ":cmeston!user@host JOIN #joli_channel\r\n", sizeof(":cmeston!user@host JOIN #joli_channel\r\n"), 0);
				// }
				ret = buff.find("\r\n");
				pos = 0;
				while (ret != -1)
				{
					std::cout << "Commande recue commence a " << pos << " et finit a " << ret - 1 << std::endl;
					std::cout << buff.substr(pos, ret - pos) << std::endl;
					pos = ret + 2;
					ret = buff.find("\r\n", ret + 2);
				}

			}

		}
		}


    /* Fermeture des sockets */

    close(clientSocket);
    close(serverSocket);

    return (0);
}