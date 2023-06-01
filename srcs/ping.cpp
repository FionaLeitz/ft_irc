#include "../headers/irc.h"

void	ft_ping(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)context;
	(void)fds;
	(void)i;
	
	std::string	response = RPL_PING(tmp->getNickname(), tmp->getUsername(), args[0]);
	send(tmp->getFd(), response.c_str(), response.length(), 0);
}

