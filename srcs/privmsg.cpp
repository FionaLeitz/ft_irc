#include "../headers/irc.h"

void	ft_privmsg(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	(void)context;
	std::cout << "Received command PRIVMSG w args " << args[0] << " and " << args[1] << std::endl;
	std::cout << "	client buff is  " << (*tmp).getBuffer() << std::endl;
	std::string dest;
	std::string	message;
	std::string	response;

	dest = args[0];
	message = args[1];
	message = (*tmp).getBuffer();
	message = message.substr(message.find(":") + 1);
	// message = message.substr(1, message.size() - 1); // retire le ':' au debut du message
	std::cout << "\tmessage = " << message << "\n\tdestinataire = " << dest << std::endl;
	response = ":" + (*tmp).getNickname() + " PRIVMSG " + dest + " :" + message +"\r\n";
	send(fds[i + 1].fd, response.c_str(), response.length(), 0);
}
