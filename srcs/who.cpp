#include "../headers/irc.h"

void	ft_who(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command WHO w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;
	std::string nick;

	nick = (*tmp).getNickname();
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		response = ":server 352 " + nick + " " + args[0] + " " + (*it).second.getUsername() + " host" + " server " + (*it).second.getNickname() + " H :0 " + "realname" + "\r\n";
		std::cout << response << std::endl;
		send(fds[i].fd, response.c_str(), response.length(), 0);
	}
	response = ":server 315 " + nick + " " + args[0] + " :End of /WHO list.\r\n";
	std::cout << response << std::endl;
	send(fds[i].fd, response.c_str(), response.length(), 0);
}
/*
response = :server 352 toto #u titi host  server titi H :0 realname
response = :server 352 toto #u toto host  server toto H :0 realname
response = :server 315 toto #u :End of /WHO list.
*/
