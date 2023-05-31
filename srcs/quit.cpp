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
	std::cout << "Received command PRIVMSG w args " << args[0] << " and " << args[1] << std::endl;
	std::string						response;
	std::map<int, Client>::iterator	it;
	(void)args;

	it = context->clients.find(fds[i].fd);
	std::string	message = (*tmp).getBuffer();
	message = message.substr(message.find(":") + 1);
	response = RPL_QUIT((*tmp).getNickname(), (*tmp).getUsername(),  tmp->getHost(), message);
	//pour tous les channels dans lequel se trouve l'utilisateur
	for (std::map<std::string, int>::const_iterator it2 = (*tmp).getChannelList().begin(); it2 != (*tmp).getChannelList().end(); ++it2)
	{
		if (it2->second == 1)
		{
			std::cout << "sending goodbye to channel " << it2->first << std::endl;
			context->channels[it2->first].sendMessage(response, (*tmp).getFd());
		}
	}
	std::cout << "RPL QUIT = " << response << std::endl;
	// context->channels[dest].sendMessage(response, (*tmp).getFd());
	context->clients.erase(it);
	close( fds[i].fd );

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