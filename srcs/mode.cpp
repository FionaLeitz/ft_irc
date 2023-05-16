#include "../headers/irc.h"

void	ft_mode(Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command MODE w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;

	response = ":server 324 " + (*tmp).getNickname() + " " + args[0] + " +nt \r\n";
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
