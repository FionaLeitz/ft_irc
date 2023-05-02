
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
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

    /* Acceptation de la première connexion entrante */

	socklen_t clientAddressLength = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket == -1)
	{
        std::cerr << "Error while executing accept() : " << strerror(errno) << std::endl;
        return(1);
    }
	else
	{
		std::cout << "Client socket successfully created" << std::endl;
	}

    /* Réception du message du client */

    char buffer[256];
    ret = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (ret == -1)
	{
        std::cerr << "Error while receiving the message with recv() : " << strerror(errno) << std::endl;
        return(1);
    }
	else
	{
		std::cout << "Successfully received a message of size " << ret << " from client ! "<< std::endl;
	}

    /* Affichage du message reçu */

    std::cout << "Message reçu : " << buffer << std::endl;

    /* Fermeture des sockets */

    close(clientSocket);
    close(serverSocket);

    return (0);
}