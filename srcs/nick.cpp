#include "../headers/irc.h"

// NICK : 	- si le nickname est deja utilise, ERR_NICKNAMEINUSE
// 		- si le nickname n'est pas valide, ERR_ERRONEUSNICKNAME (peut etre interdire # : et espace (ca se gere deja je crois))
// 		- hexchat : split automatiquement avec l'espace;  accepte ':' ;  accepte '#';
// 		- si pas d'argument (pas de nickname communique) ERR_NONICKNAMEGIVEN
// 				"The NICK message may be sent from the server to clients to
// 				acknowledge their NICK command was successful, and to inform
// 				other clients about the change of nickname. In these cases,
// 				the <source> of the message will be the old nickname
// 				[ [ "!" user ] "@" host ] of the user who is changing their nickname."

void	ft_nick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	(void)i;
	(void)fds;
	std::cout << "Received command NICK" << std::endl;// w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;
	std::string	RPLnick;
	std::map<int, Client>::const_iterator it;
	std::string oldNick;

	if (args.size() == 0)
	{
		response = ERR_NONICKNAMEGIVEN();
		send(tmp->getFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		// check caracteres interdits
		if (args[0].find_first_of("#:") != std::string::npos)
		{
			response = ERR_ERRONEUSNICKNAME(args[0]);
			std::cout << "Invalid char in nick" << std::endl;
			if (tmp->getNickname().empty())
				(*tmp).setNickname(args[0]);
			tmp->setCanConnect(false);
		}
		else
		{
			// check si dispo
			for (it = context->clients.begin(); it != context->clients.end(); ++it)
			{
				std::cout << "Comparing " << args[0] << " to " << it->second.getNickname()<< std::endl;
				if (args[0] == it->second.getNickname())
				{
					std::cout << "Nickname deja pris par user " << it->first << " (" << USER_ID(it->second.getNickname(), it->second.getUsername(), it->second.getHost()) << ")" << std::endl;
					response = ERR_NICKNAMEINUSE(args[0]);
					if (tmp->getNickname().empty())
						(*tmp).setNickname(args[0]);
					tmp->setCanConnect(false);
					break ;
				}
			}
			// dispo 
			if (it == context->clients.end())
			{
				std::cout << "Nickname dispo" << std::endl;
				if (oldNick.empty()){
					oldNick = tmp->getNickname();
					if (oldNick.empty())
						oldNick = args[0];
				}
				response = RPL_NICK(oldNick, tmp->getUsername(), tmp->getHost(), args[0]);
				(*tmp).setNickname(args[0]);
				tmp->setCanConnect(true);
			}
		}
		std::cout << "RPL = " << response << std::endl;
		send(tmp->getFd(), response.c_str(), response.length(), 0);
	}
}