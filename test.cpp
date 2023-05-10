
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <poll.h>

#define USER_ID(nickname, username) (nickname + "!" + username + "@host")
#define RPL_WELCOME(nickname, username) (":server 001 " + nickname + " :Welcome to the test IRC Network " + USER_ID(nickname, username) + "\r\n")
#define RPL_JOIN(nickname, username, channel) (":" + USER_ID(nickname, username) + " JOIN " + channel + "\r\n")

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
	std::string			nick;
	std::string			username;
	std::string			response;
	std::string			message;

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
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // conversion depuis l'ordre des octets de l'hôte vers celui du réseau (pour un uint32)
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
			}
			else
			{
				std::cout << "Successfully received a message of size " << ret << " from client ! "<< std::endl;
				std::cout << "Message reçu : " << buff << std::endl;
				if(buff.find("USER") != std::string::npos)
				{
					// #IRC connection handshake consists of sending NICK and USER messages. All IRC messages must end in \r\n
					ret = buff.find("NICK") + 5;
					nick = buff.substr(ret, buff.find("USER") - 2 - ret);
					ret = buff.find("USER") + 5;
					username = buff.substr(ret, buff.find(" ", ret + 5) - ret);
					response = RPL_WELCOME(nick, username);
					std::cout << "\tnickname = " << nick << "\n\tusername = " << username << std::endl;
					send(clientSocket, response.c_str(), response.length(), 0);
					 //jsp si utile ou pas ?
				}
				if (buff.find("coucou") != std::string::npos)
				{
					std::cout << "!!!!!!!!!" << std::endl;
					response = RPL_JOIN(nick, username, "#joli_channel");
					// std::cout << "###########" << std::endl << response << std::endl << RPL_JOIN(nick, username, "#joli_channel") << std::endl << std::endl;
					send(clientSocket, response.c_str(), response.length(), 0);
				}
				else if (buff.find("MODE") != std::string::npos)
				{
					response = ":server 324 " + nick + " #joli_channel +nt -i\r\n";
					send(clientSocket, response.c_str(), response.length(), 0);
				}
				else if (buff.find("WHO") != std::string::npos)
				{
					response = ":server 352 " + nick + " #joli_channel " + nick + " user host server " + nick + " H :0 " + nick + "\r\n";
					send(clientSocket, response.c_str(), response.length(), 0);
				}
				else if (buff.find("PRIVMSG") != std::string::npos)
				{
					ret = buff.find("#joli_channel :") + 15;
					message = buff.substr(ret, buff.find("\r\n") - ret);
					std::cout << "message = " << message << std::endl;
					response = ":server PRIVMSG #joli_channel :" + message +"\r\n";
					send(clientSocket, response.c_str(), response.length(), 0);
				}
				ret = buff.find("\r\n");
				pos = 0;
				
				while (ret != -1)
				{
					std::cout << "Commande recue (buff[" << pos << "] -- buff[" << ret - 1 << "]) : " 
					<< buff.substr(pos, ret - pos) << std::endl;
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