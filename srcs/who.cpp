#include "../headers/irc.h"

void	ft_who(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	reply;
	std::string nick;

	if (args.size() == 0)
	{
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "WHO");
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	nick = (*tmp).getNickname();
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		reply = RPL_WHOREPLY(nick, args[0], (*it).second.getUsername(), (*it).second.getHost(), (*it).second.getNickname());
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
	}
	reply = RPL_ENDOFWHO(nick, args[0]);
	send(fds[i].fd, reply.c_str(), reply.length(), 0);
}

