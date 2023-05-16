#include "../headers/irc.h"

void	ft_nick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	(void)context;
	std::cout << "Received command NICK w args " << args[0] << " and " << args[1] << std::endl;
	// std::string	response;
	(void)i;
	(void)fds;

	(*tmp).setNickname(args[0]);
	// send(fds[i].fd, response.c_str(), response.length(), 0);
}
