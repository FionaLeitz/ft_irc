#include "../headers/irc.h"

void	ft_pass(Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command PASS w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;

	(*tmp).setUsername(args[0]);
	response = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername());
	send(fds[i].fd, response.c_str(), response.length(), 0);
}