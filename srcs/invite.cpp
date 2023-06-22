#include "../headers/irc.h"

void	ft_invite(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;

	std::string reply;
	int			fd;
	if (args.size() != 2) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "INVITE");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	if ( context->channels.find(args[1]) == context->channels.end() ) {
		reply = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[1]);
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		return ;
	}
	if (context->channels[args[1]].isUserOperator(*tmp) == false)
		return ;
	reply = RPL_INVITE_MSG(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1]);
	fd = findClientFd(context->clients, args[0]);
	if (fd == -1 )
		reply = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[1], args[0]);
	else
	{
		send(fd, reply.c_str(), reply.size(), 0);
		reply = RPL_INVITING(tmp->getNickname(), args[1], args[0]);
		context->clients[fd].addInviteChannel(args[1]);
	}
	send(tmp->getFd(), reply.c_str(), reply.size(), 0);
}
