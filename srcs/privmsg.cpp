#include "../headers/irc.h"

int	nickname_fd( std::string nickname, std::map<int, Client> clients ) {
	std::map<int, Client>::iterator	it = clients.begin();
	std::map<int, Client>::iterator	ite = clients.end();

	for ( ; it != ite; it++ ) {
		if ( it->second.getNickname() == nickname )
			return it->first;
	}
	return -1;
}

void	ft_privmsg(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string dest;
	std::string	message;
	std::string	reply;
	(void)fds;
	(void)i;

	if ( args[1].empty() ) {
		reply = ERR_NOTEXTTOSEND(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	dest = args[0];
	message = (*tmp).getBuffer();
	message = message.substr(message.find(dest) + dest.length() + 1);
	if (message[0] == ':')
		message = message.substr(1, message.size() - 1); 
	reply = ":" + (*tmp).getNickname() + " PRIVMSG " + dest + " :" + message +"\r\n";
	if (dest[0] == '#')
	{
		if ( context->channels.find(dest) != context->channels.end() )
			context->channels[dest].sendMessage(reply, (*tmp).getFd());
		else {
			reply.clear();
			reply = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), dest);
			send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		}
	}
	else {
		int	fd = nickname_fd( dest, context->clients );
		if ( fd != -1 )
			send(fd, reply.c_str(), reply.length(), 0);
		else {
			reply.clear();
			reply = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "", dest);
			send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		}
	}
}
