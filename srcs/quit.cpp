#include "../headers/irc.h"

// void	ft_quit( std::map<int, Client> clients, int fd, std::string message, int fd2 ) {
// 	std::map<int, Client>::iterator	it = clients.find( fd );
// 	std::string	response = RPL_QUIT(it->second.getNickname(), it->second.getUsername(), message);
// 	std::cout << "RPL QUIT = " << response << std::endl;
// 	// envoyer a tous les channels du client qui QUIT
// 	send(fd2, response.c_str(), response.length(), 0);
// 	close( fd );
// 	clients.erase( fd );
// }

void	ft_quit(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	// std::cout << "On essaye de QUIT la !" << std::endl;
	std::cout << "Received command PRIVMSG w args " << args[0] << " and " << args[1] << std::endl;
	std::string						response;
	std::map<int, Client>::iterator	it;
	int								j;
	(void)args;

	it = context->clients.find(fds[i].fd);
	std::string	message = (*tmp).getBuffer();
	message = message.substr(message.find(":") + 1);
	response = RPL_QUIT((*tmp).getNickname(), (*tmp).getUsername(), message);
	send(fds[i + 1].fd, response.c_str(), response.length(), 0);
	std::cout << "RPL QUIT = " << response << std::endl;
	close( fds[i].fd );
	context->clients.erase(fds[i].fd);
	(void)j;

	// envoyer a tous les channels du client qui QUIT
	// while (fds[j])
	// 	{
	// 	if (fds[j].fd != fds[i].fd)
	// 		send(fds[j].fd, response.c_str(), response.length(), 0);
	// 	j++;
	// }
	// context->clients.erase(fds[i].fd);
	// close(fds[i].fd);
}