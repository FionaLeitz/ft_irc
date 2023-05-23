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

void	ft_privmsg(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command PRIVMSG w args " << args[0] << " and " << args[1] << std::endl;
	std::cout << "	client buff is  " << (*tmp).getBuffer() << std::endl;
	std::string dest;
	std::string	message;
	std::string	response;
	(void)fds;
	(void)i;

	if ( args[1].empty() ) {
		response = ERR_NOTEXTTOSEND(tmp->getNickname(), tmp->getUsername() );
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	dest = args[0];
	message = (*tmp).getBuffer();
	message = message.substr(message.find(dest) + dest.length() + 1);
	if (message[0] == ':')		// retire le ':' au debut du message s'il y en a un
		message = message.substr(1, message.size() - 1); 
	std::cout << "\tmessage = " << message << "\n\tdestinataire = " << dest << std::endl;
	response = ":" + (*tmp).getNickname() + " PRIVMSG " + dest + " :" + message +"\r\n";
	if (dest[0] == '#')
	{
		if ( context->channels.find(dest) != context->channels.end() )
			context->channels[dest].sendMessage(response, (*tmp).getFd());
		else {
			response.clear();
			response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), dest);
			send(tmp->getFd(), response.c_str(), response.length(), 0);
		}
	}
	else {
		int	fd = nickname_fd( dest, context->clients );
		if ( fd != -1 )
			send(fd, response.c_str(), response.length(), 0);
		else {
			response.clear();
			response = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), dest);
			send(tmp->getFd(), response.c_str(), response.length(), 0);
		}
	}
}
