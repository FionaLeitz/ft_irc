#include "../headers/irc.h"

void	ft_quit( std::map<int, Client> clients, int fd, std::string message, int fd2 ) {
	std::map<int, Client>::iterator	it = clients.find( fd );
	std::string	response = RPL_QUIT(it->second.getNickname(), it->second.getUsername(), message);
	std::cout << "RPL QUIT = " << response << std::endl;
	// envoyer a tous les channels du client qui QUIT
	send(fd2, response.c_str(), response.length(), 0);
	close( fd );
	clients.erase( fd );
}
