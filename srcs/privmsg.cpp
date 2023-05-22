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

	dest = args[0];
	message = args[1];
	message = (*tmp).getBuffer();
	// message = message.substr(message.find(dest) + dest.size() + 1);
	message = message.substr(1, message.size() - 1); // retire le ':' au debut du message
	std::cout << "\tmessage = " << message << "\n\tdestinataire = " << dest << std::endl;
	response = ":" + (*tmp).getNickname() + " PRIVMSG " + dest + " :" + message +"\r\n";
	if (dest[0] == '#')
	{
		context->channels[dest].sendMessage(response, (*tmp).getFd());
		// sendToAllClients(context->channels[dest].getClientlist(), message);
	}
	else {
		int	fd = nickname_fd( dest, context->clients );
		message.clear();
		message = (*tmp).getBuffer();
		message = message.substr(message.find(dest) + dest.size() + 1);
		response.clear();
		if ( message[0] == ':')
			message = message.substr(1, message.size() - 1);
		response = ":" + (*tmp).getNickname() + " PRIVMSG " + dest + " :" + message +"\r\n";
		if ( fd != -1 )
			send(fd, response.c_str(), response.length(), 0);
		else
			std::cout << "ERROR PRIVMSG : nickname does not exist" << std::endl;
	}
}

		// response = ":" + (*tmp).getNickname() + " " + message + "\r\n";
		// int	fd = nickname_fd( dest, context->clients );
		// if ( fd != -1 )
		// 	send(fd, response.c_str(), response.length(), 0);