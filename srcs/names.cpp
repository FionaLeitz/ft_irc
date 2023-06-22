#include "../headers/irc.h"

void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	reply;
	std::string nick;
	std::string oname;
	(void)fds;
	(void)i;

	if (args.size() < 1) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "NAMES");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	nick = (*tmp).getNickname();
	if (context->channels.find(args[0]) == context->channels.end()) {
		reply = ERR_NOSUCHCHANNEL(nick, tmp->getUsername(), tmp->getHost(), args[0]);
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		oname = it->second.getNickname();
		if (context->channels[args[0]].getOperators().find(oname) != context->channels[args[0]].getOperators().end())
			oname.insert(oname.begin(), '@');
		reply = RPL_NAMREPLY(nick, "=", args[0], "", oname);
		// send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		context->channels[args[0]].sendToAll(reply);
	}
}
