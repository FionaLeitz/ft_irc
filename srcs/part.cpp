#include "../headers/irc.h"

void	ft_part(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	int	count = 0;
	std::string	message;

	for ( std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++ ) {
		if (count != 0) {
			if (count == 1)
				message = &(*it)[1];
			else
				message += *it;
			message += " ";
		}
		count++;
	}
	std::vector<std::string>	chans = ft_split( args[0], "," );
	std::string					reply;
	for ( std::vector<std::string>::iterator 			it = chans.begin(); it != chans.end(); it++ ) {
		std::map<std::string,int>::const_iterator		finding = tmp->getChannelList().find(*it);
		std::map<std::string, Channel>::iterator		existingchannel = context->channels.find( *it );
		if ( existingchannel == context->channels.end() ) {
			reply = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), *it );
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		}
		else if ( finding == tmp->getChannelList().end() || finding->second == 0 ) {
			reply = ERR_NOTONCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), *it );
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		}
		else {
			reply = RPL_PART(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), *it, message);
			existingchannel->second.sendToAll( reply);
			tmp->removeChannel( context, *it );
			existingchannel->second.suppress_client( tmp->getNickname() );
			suppress_empty_chan( context, *it );
		}
	}
}
