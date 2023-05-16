#include "../headers/irc.h"

void	ft_user(Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command USER w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;

	if ( (*tmp).getUsername() != "\0" ) {
		// std::cout << "ERROR !!!" << std::endl;
		response = ":server 462 " + USER_ID( (*tmp).getNickname(), (*tmp).getUsername() ) + " :You may not reregister\r\n";
		send(fds[i].fd, response.c_str(), response.length(), 0);
		return ;
	}
	(*tmp).setUsername(args[0]);
	response = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername());
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
