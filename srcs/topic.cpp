#include "../headers/irc.h"

void	check_topic(Client *tmp, Channel &channel)
{
	std::string reply;

	if (channel.getTopic().empty())
		reply = RPL_NOTOPIC(tmp->getNickname(), channel.getName());
	else
		reply = 	RPL_TOPIC(tmp->getNickname(), channel.getName(), channel.getTopic());
	send(tmp->getFd(), reply.c_str(), reply.length(), 0);
}

void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {

/*
  
  Parameters: <channel> [<topic>]

	1.	If <topic> is not given (check topic) :
		- either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or
		lack of one
			- if RPL_TOPIC is returned to the client sending this command, RPL_TOPICWHOTIME SHOULD also be sent to that client.
	2. 	If <topic> is an empty string (clear topic):
		- the topic for the channel will be cleared
		- every client in that channel (including the author of the topic change) will receive a TOPIC command with an
		empty argument alerting them to how the topic has changed
		- clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly
	3. Else (set topic):
		- every client in that channel (including the author of the topic change) will receive a TOPIC command with the
		new topic as argument alerting them to how the topic has changed.
		- clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly
	
	- If the client sending this command is not joined to the given channel, and tries to view its’ topic, the server
		MAY return the ERR_NOTONCHANNEL (442) numeric and have the command fail.
	(-  If the protected topic mode is set on a channel, then clients MUST have appropriate channel permissions to modify
		the topic of that channel. If a client does not have appropriate channel permissions and tries to change the
			topic, the ERR_CHANOPRIVSNEEDED (482) numeric is returned and the command will fail.)
	
	TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic".
  	TOPIC #test :                   ; Clearing the topic on "#test"
  	TOPIC #test                     ; Checking the topic for "#test"
*/
	(void)fds;
	(void)i;
	std::string reply;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the topic command" << std::endl;

	if (args.size() == 1)
	{
		//check topic
		// if (context->channels[args[0]].getTopic().empty())
		// 	reply = RPL_NOTOPIC(tmp->getNickname(), args[0]);
		// else
		// 	reply = 	RPL_TOPIC(tmp->getNickname(), args[0], context->channels[args[0]].getTopic());
		// send(fds[i].fd, reply.c_str(), reply.length(), 0);
		check_topic(tmp, context->channels[args[0]]);
	}
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
		// :cmeston!~cmeston@ecc-b03d-a3d6-12a8-9792.210.62.ip TOPIC #aaaa :  
	}
	// context->channels[args[1]].sendToAll(reply);
}
/*
>> /topic
<< #u :No topic is set.
>> /topic [space][space]
<< toto has changed the topic to:  
>> /topic
<<  Topic for #u is:  
* Topic for #u set by toto!t@IRC4Fun-ucb.f6d.210.62.IP (Thu May 25 18:46:38 2023)
>> /topic Bonjour
<<  toto has changed the topic to: Bonjour
>> /topic
<< Topic for #z is: Bonjour
* Topic for #z set by toto!t@IRC4Fun-ucb.f6d.210.62.IP (Thu May 25 18:48:23 2023)
*/