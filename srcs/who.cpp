#include "../headers/irc.h"

void	ft_who(Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command WHO w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;

	response = ":server 352 " + (*tmp).getNickname() + " " + args[0] + " " + (*tmp).getNickname() + " user host server toto" + (*tmp).getNickname() + " H :0 " + (*tmp).getNickname() + "\r\n";
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
