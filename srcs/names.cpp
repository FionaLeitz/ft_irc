#include "../headers/irc.h"

void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	reply;
	std::string nick;
	std::string oname;
	(void)fds;
	(void)i;

	// names prend forcement un argument ?
	nick = (*tmp).getNickname();
	for (std::map<std::string, Client>::const_iterator it = context->channels[args[0]].getClientlist().begin(); it != context->channels[args[0]].getClientlist().end(); ++it) {
		oname = it->second.getNickname();
		if (context->channels[args[0]].getOperators().find(tmp->getNickname()) != context->channels[args[0]].getOperators().end())
			oname.insert(oname.begin(), '@');
		reply = RPL_NAMREPLY(nick, "=", args[0], "", oname);
		std::cout << reply << std::endl;
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		// :server 353 navierxiel = #i :navierxiel
		// :sakura.jp.as.dal.net 353 tuputu = #dskljfskljldj :@tuputu 
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