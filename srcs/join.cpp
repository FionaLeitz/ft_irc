#include "../headers/irc.h"

void	ft_join(Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::string	response;

	std::cout << "Received command JOIN w args " << args[0] << " and " << args[1] << std::endl;
	std::cout << (*tmp).getNickname() << " veut rejoindre le channel" << args[0] << std::endl;
	response = RPL_JOIN((*tmp).getNickname(), (*tmp).getUsername(), args[0]);
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
