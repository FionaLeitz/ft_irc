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

void	ft_quit(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command PRIVMSG" << std::endl;// w args " << args[0] << " and " << args[1] << std::endl;
	std::string						response;
	std::map<int, Client>::iterator	it;
	(void)args;

	it = context->clients.find(fds[i].fd);
	std::string	message = (*tmp).getBuffer();
	message = message.substr(message.find(":") + 1);
	response = RPL_QUIT((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost(), message);
	//pour tous les channels dans lequel se trouve l'utilisateur
	// for (std::map<std::string, int>::const_iterator it2 = (*tmp).getChannelList().begin(); it2 != (*tmp).getChannelList().end(); ++it2)
	// {
	// 	if (it2->second == 1)
	// 	{
	// 		std::cout << "sending goodbye to channel " << it2->first << std::endl;
			// context->channels[it2->first].sendMessage(response, (*tmp).getFd());
	// 		context->channels[it2->first].suppress_client(tmp->getNickname());
	// 	}
	// }
	tmp->leaveAllChannels(context, response);
	std::cout << "RPL QUIT = " << response << std::endl;
	context->clients.erase(it);
	// it = context->clients.find(fds[i].fd);
	// if (it == context->clients.end())
	// 	std::cout << "C'est bon il n'est plus trouvable" << std::endl;
	// else
	// 	std::cout << "C'est pas normal ca pue" << std::endl;
	close( fds[i].fd );
}