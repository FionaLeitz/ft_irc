#include "../headers/irc.h"

void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)fds;
	(void)i;

	std::string channel;
	std::string user;
	std::string reason;

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
		std::cout << "No such channel" << std::endl; //remplacer par send err
		return ;
		// ERR_NOSUCHCHANNEL (403)
//   "<client> <channel> :No such channel"

	}
	//si channel ok, trouver user dans le channel
	if (!context->channels[args[0]].isUserThere(args[1]))
	{
		std::cout << "No such user" << std::endl; //remplacer par send err
		return ;
		// ERR_USERNOTINCHANNEL (441)
  		// "<client> <nick> <channel> :They aren't on that channel"
		// Returned when a client tries to perform a channel+nick affecting command, when the nick isn’t joined to the channel (for example, MODE #channel +o nick).
	}
	std::cout << "Kicking " << args[1] << std::endl;




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
// tutusse has kicked tutux from #waoooooow (tutusse)
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