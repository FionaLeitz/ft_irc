#include "../headers/irc.h"

void	check_topic(Client *tmp, Channel &channel)
{
	std::string reply;

	if (channel.getTopic().empty())
		reply = RPL_NOTOPIC(tmp->getNickname(), channel.getName());
	else
		reply = RPL_TOPIC(tmp->getNickname(), channel.getName(), channel.getTopic());
	send(tmp->getFd(), reply.c_str(), reply.length(), 0);
}

void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	std::string reply;

	if (args.size() < 1) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "TOPIC");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	if (args.size() == 1)
		check_topic(tmp, context->channels[args[0]]);
	else
	{
		for(size_t i = 2; i < args.size(); i++) {
			args[1].append(" ");
			args[1].append(args[i]);
		}
		std::cout << args.size() << std::endl;

		// change topic
		if (context->channels[args[0]].getMode().find('t') != std::string::npos) {
		 	if (context->channels[args[0]].isUserOperator(*tmp) == false)
				return ;
		}
		
		reply = RPL_CHANGETOPIC(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1]);
		if (args[1].size() > 1)
			context->channels[args[0]].setTopic(args[1].substr(1, args[1].size() - 1));
		else
			context->channels[args[0]].setTopic(" ");
		context->channels[args[0]].sendToAll(reply);
	}
}
