#include "../headers/irc.h"

void	ft_user(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	(void)context;
	std::cout << "Received command USER" << std::endl;
	std::string	reply;


	if (args.size() == 0) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "USER");
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if ( (*tmp).getUsername() != "\0" && tmp->canConnect() == 2) {
		reply = ERR_ALREADYREGISTERED(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		std::cout << reply << std::endl;
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	tmp->setUsername(args[0]);
	if (tmp->canConnect() != 1)
		return ;
	reply = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost());
	send(fds[i].fd, reply.c_str(), reply.length(), 0);
	tmp->setCanConnect(2);
}
