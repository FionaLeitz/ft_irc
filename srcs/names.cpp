#include "../headers/irc.h"

void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	reply;
	std::string nick;
	std::string oname;
	(void)fds;
	(void)i;

	nick = (*tmp).getNickname();
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		oname = it->second.getNickname();
		if (context->channels[args[0]].getOperators().find(tmp->getNickname()) != context->channels[args[0]].getOperators().end())
			oname.insert(oname.begin(), '@');
		reply = RPL_NAMREPLY(nick, "=", args[0], "", oname);
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
	}
}
