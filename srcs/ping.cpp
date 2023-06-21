#include "../headers/irc.h"

void	ft_ping(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)context;
	(void)fds;
	(void)i;
	
	std::string	reply = RPL_PING(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0]);
	send(tmp->getFd(), reply.c_str(), reply.length(), 0);
}

