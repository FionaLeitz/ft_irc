#include "../headers/irc.h"

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

	tmp->leaveAllChannels(context, response);
	std::cout << "RPL QUIT = " << response << std::endl;
	context->clients.erase(it);
	close( fds[i].fd );
}