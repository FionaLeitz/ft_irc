#include "../headers/irc.h"

// void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
// 	(void)context;
// 	(void)fds;
// 	(void)i;
// 	(void)args;

// 	std::cout << "Client "<<tmp->getNickname() << " is trying to use the names command." << std::endl;
// }

// // void	names_all( Client client, std::map<std::string, Client> clients) {
// // 	std::map<std::string, Client>::iterator	it = clients.begin();
// // 	std::map<std::string, Client>::iterator	ite = clients.end();

// // 	for ( ; it != ite; it++ ) {
// // 		// envoyer au client it->first;
// // 	}
// // }

// // void	names_channel( Client client, Channel channel ) {
// // 	std::map<std::string, Client>::iterator	it = channel.getClientlist.begin();
// // 	std::map<std::string, Client>::iterator	ite = channel.getClientlist.end();

// // 	for ( ; it != ite; it++ ) {
// // 		// envoyer au client it->first;
// // 	}
// // }

void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::string	response;
	std::string nick;
	(void)fds;
	(void)i;

	nick = (*tmp).getNickname();
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		response = RPL_NAMREPLY(nick, "=", args[0], "", it->second.getNickname());
		std::cout << response << std::endl;
		send(tmp->getFd(), response.c_str(), response.length(), 0);
	}
}

/*
<symbol> notes the status of the channel. It can be one of the following:

    ("=", 0x3D) - Public channel.
    ("@", 0x40) - Secret channel (secret channel mode "+s").
    ("*", 0x2A) - Private channel (was "+p", no longer widely used today).

<nick> is the nickname of a client joined to that channel, and <prefix> is the highest channel membership prefix that client has in the
channel, if they have one. The last parameter of this numeric is a list of [prefix]<nick> pairs, delimited by a SPACE character (' ', 0x20)
*/