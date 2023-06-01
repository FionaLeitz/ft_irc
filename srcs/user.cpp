#include "../headers/irc.h"

void	ft_user(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	(void)context;
	std::cout << "Received command USER" << std::endl;// w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;

	if ( (*tmp).getUsername() != "\0" ) {
		response = ":server 462 " + USER_ID( (*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost()) + " :You may not reregister\r\n";
		send(fds[i].fd, response.c_str(), response.length(), 0);
		return ;
	}
	(*tmp).setUsername(args[0]);
	response = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost());
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
