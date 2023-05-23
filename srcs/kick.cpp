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


// << MODE #ez +o tuutuutu
// >> :cmeston_!~cmeston@2618-2ed8-f8de-36d4-9088.210.62.ip MODE #ez +o tuutuutu
// << WHO #ez
// >> :hash.ix.nl.dal.net 352 cmeston_ #ez ~cmeston 2618-2ed8-f8de-36d4-9088.210.62.ip punch.wa.us.dal.net tuutuutu H@ :4 Chine MESTON
// >> :hash.ix.nl.dal.net 352 cmeston_ #ez ~cmeston 2618-2ed8-f8de-36d4-9088.210.62.ip hash.ix.nl.dal.net cmeston_ H@ :0 Chine MESTON

void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)fds;
	(void)i;

	std::string channel;
	std::string user;
	std::string reason;
	std::string response;

	std::cout << "Client "<< tmp->getNickname() << " is trying to use the KICK command w args " << args[0] << " and " << args[1] << std::endl;

	// ERR_NEEDMOREPARAMS (461)
//   "<client> <command> :Not enough parameters"

	channel = args[0];
	user = args[1];
	reason = ((*tmp).getBuffer());
	reason = reason.substr(reason.find(args[1]));
	reason = reason.substr(args[1].length() + 1);
	std::cout << "Trying to kick " << user << " from channel " << channel << " with reason : " << reason << std::endl;

	//droits 
		// ERR_CHANOPRIVSNEEDED (482)
		//   "<client> <channel> :You're not channel operator"
		// Indicates that a command failed because the client does not have the appropriate channel privileges. This numeric can apply for different prefixes such as halfop, operator, etc. The text used in the last param of this message may vary.

	//trouver le channel
	if (context->channels.find(args[0]) == context->channels.end())
	{
		response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), args[0]);
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	//si channel ok, trouver user dans le channel
	else if (!context->channels[args[0]].isUserThere(args[1]))
	{
		response = ERR_USERNOTINCHANNEL(tmp->getNickname(), tmp->getUsername(), args[0], args[1]);
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	else
	{
		response = RPL_KICK(tmp->getNickname(), tmp->getUsername(), args[0], args[1], reason);
		context->channels[args[0]].sendToAll(response);
		context->channels[args[0]].suppress_client(args[1]);
		int fd = findClientFd(context->clients, args[1]);
		std::cout << "Kicking " << args[1] << " (fd = " << fd << ")" << std::endl;
		context->clients[fd].removeChannel(args[0]);
		// >> :totousse!~cmeston@2618-2ed8-f8de-36d4-9088.210.62.ip KICK #wewewe cmeston_ :totousse
	}
}

// void	kick( Client operator, std::string nick, Channel channel ) {
// 	// verifier que l'operateur (le kickeur) ait les droits pour le faire
// 	if ( channel.getOperators().find( operator.getNickname() ) == channel.getOperators().end() )
// 		return ;
// 	channel.suppress_client( nick );
// }



// erreurs possibles :

// #waoooooow :No such nick/channel
//  Usage: KICK <nick> [reason], kicks the nick from the current channel (needs chanop)


// in channel : KICK user
//  #waoooooow :No such nick
//  #waoooooow :You're not channel operator / #wewewe :You must be a channel op or higher to kick a more privileged user.
// tutusse has kicked tutux from #waoooooow (tutusse) / !- cmeston_ was kicked from #wewewe by totousse [totousse]
//  You have been kicked from #waoooooow by tutusse (tutusse

/*
dans notre serveur :

>> /kick #wewe toto
	<< KICK 3615 #wewe :toto
>> /kick #wewe toto :bye
	<< KICK 3615 #wewe :toto :bye
(inside of channel #wewe)
>> /kick toto
	<< KICK #u toto
>> /kick #other toto
	<< KICK #u #other :toto

   Command: KICK
   Parameters: <channel> <user> *( "," <user> ) [<comment>]
   Causes the <user> to be removed from the <channel> by force. If no comment is given, the server SHOULD use a default message instead.
*/