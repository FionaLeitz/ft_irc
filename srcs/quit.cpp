#include "../headers/irc.h"

void	ft_quit(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string						reply;
	std::map<int, Client>::iterator	it;
	(void)args;

	it = context->clients.find(fds[i].fd);
	std::string	message = (*tmp).getBuffer();
	message = message.substr(message.find(":") + 1);
	reply = RPL_QUIT((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost(), message);

	tmp->leaveAllChannels(context, reply);
	context->clients.erase(it);
	if (fds[i].fd) {
		close( fds[i].fd );
		fds[i].fd = -1;
	}
}