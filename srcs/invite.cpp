#include "../headers/irc.h"

void	ft_invite(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)fds;
	(void)i;

	std::string response;
	int			fd;
	std::cout << "Client "<<tmp->getNickname() << " is trying to use the invite command w args " << args[0] <<  " and " << args[1] << std::endl;
	if (context->channels[args[1]].isUserOperator(*tmp) == false)
		return ;
	response = RPL_INVITE_MSG(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1]);
	fd = findClientFd(context->clients, args[0]);
	if (fd == -1 )
		response = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0]);
	else
	{
		send(fd, response.c_str(), response.size(), 0);
		response = RPL_INVITING(tmp->getNickname(), args[1], args[0]);
		context->clients[fd].addInviteChannel(args[1]);
	}
	send(tmp->getFd(), response.c_str(), response.size(), 0);
}
