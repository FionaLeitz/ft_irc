#include "../headers/irc.h"

void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {

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

	(void)context;
	std::string response;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the topic command w args " << args[0] << " and " << args[1] << std::endl;
	std::cout << "length == "<< args[1].length() << std::endl;
	if (args[1].empty())
	{
		//check topic
		if (context->channels[args[1]].getTopic().empty())
			response = RPL_NOTOPIC(tmp->getNickname(), args[0]);
		else
			response = 	RPL_TOPIC(tmp->getNickname(), args[0], context->channels[args[0]].getTopic());
		send(fds[i].fd, response.c_str(), response.length(), 0);
	}
		/*
		if (context->channels[args[1]].getTopic().empty())
			response = RPL_NOTOPIC(tmp->getNickname(), tmp->getUsername(),args[0]);
		else
			response = RPL_TOPIC(tmp->getNickname(), tmp->getUsername(), args[0], context->channels[args[0]].getTopic());
	}
	else if (args[1].length() == 1)
	{
		//clear topic
		response = RPL_CHANGETOPIC(tmp->getNickname(), tmp->getUsername(), args[0], args[1]);
		context->channels[args[1]].setTopic(args[1].substr(1, args[1].size() - 1));
		context->channels[args[1]].sendToAll(response);
		response = RPL_TOPIC(tmp->getNickname(), tmp->getUsername(), args[0], context->channels[args[0]].getTopic());
	}
	else
	{
		// change topic
		response = RPL_CHANGETOPIC(tmp->getNickname(), tmp->getUsername(), args[0], args[1]);
		context->channels[args[1]].setTopic(args[1].substr(1, args[1].size() - 1));
		context->channels[args[1]].sendToAll(response);
		response = RPL_TOPIC(tmp->getNickname(), tmp->getUsername(), args[0], context->channels[args[0]].getTopic());
		// :cmeston!~cmeston@ecc-b03d-a3d6-12a8-9792.210.62.ip TOPIC #aaaa :  
	}
	std::cout << "Reply = "<< response << std::endl;
	context->channels[args[1]].sendToAll(response);
	*/
}
