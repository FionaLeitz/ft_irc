#include "../headers/irc.h"

int		findClientFd(const std::map<int, Client> &clientList, std::string nick)
{
		std::map<int, Client>::const_iterator it;
		for (it = clientList.begin(); it != clientList.end(); ++it)
		{
			if (it->second.getNickname() == nick)
				return (it->first);
		}
		return (-1);
}

void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;

	std::string channel;
	std::string user;
	std::string reason;
	std::string reply;

	if (args.size() < 2) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "KICK");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	if (context->channels[args[0]].isUserOperator(*tmp) == false)
		return ;
	channel = args[0];
	user = args[1];
	reason = ((*tmp).getBuffer());
	reason = reason.substr(reason.find(args[1]));
	reason = reason.substr(args[1].length() + 1);

	//trouver le channel
	if (context->channels.find(args[0]) == context->channels.end())
	{
		reply = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0]);
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	//si channel ok, trouver user dans le channel
	else if (!context->channels[args[0]].isUserThere(args[1]))
	{
		reply = ERR_USERNOTINCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1]);
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	else
	{
		reply = RPL_KICK(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1], reason);
		context->channels[args[0]].sendToAll(reply);
		context->channels[args[0]].suppress_client(args[1]);
		int fd = findClientFd(context->clients, args[1]);
		context->clients[fd].removeChannel(context, args[0]);
	}
}

