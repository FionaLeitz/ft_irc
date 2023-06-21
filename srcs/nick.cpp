#include "../headers/irc.h"

bool	isStringAlnum(std::string s)
{
	for (unsigned long i = 0; i < s.size(); i++)
	{
		if (!isalnum(s[i]) && s[i] != '-' && s[i] != '_')
			return false;
	}
	return true;
}

void	ft_nick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	(void)i;
	(void)fds;
	std::cout << "Received command NICK" << std::endl;
	std::string	reply;
	std::string	RPLnick;
	std::map<int, Client>::const_iterator it;
	std::string oldNick;
	std::vector<std::string> user_args;

	if (tmp->getPassBool() == false)
	{
		// rajouter RPL d'erreur
		reply = ERR_UNKOWNERROR((std::string)"nickname", (std::string)"username", tmp->getHost(), args[0], (std::string)"You need to set the password first");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	if (args.size() == 0)
	{
		reply = ERR_NONICKNAMEGIVEN();
	}
	else
	{
		// check caracteres interdits
		if (isStringAlnum(args[0]) == false)
		{
			reply = ERR_ERRONEUSNICKNAME(args[0]);
			std::cout << "Invalid char in nick" << std::endl;
			if (tmp->getNickname().empty())
				(*tmp).setNickname(args[0]);
			if (tmp->canConnect() != 2)
				tmp->setCanConnect(0);
		}
		else
		{
			// check si dispo
			for (it = context->clients.begin(); it != context->clients.end(); ++it)
			{
				if (it->second.getFd() != tmp->getFd())
					std::cout << "Comparing " << args[0] << " to " << it->second.getNickname()<< std::endl;
				if (args[0] == it->second.getNickname() && it->second.getFd() != tmp->getFd())
				{
					std::cout << "Nickname deja pris par user " << it->first << " (" << USER_ID(it->second.getNickname(), it->second.getUsername(), it->second.getHost()) << ")" << std::endl;
					reply = ERR_NICKNAMEINUSE(args[0]);
					if (tmp->getNickname().empty())
						(*tmp).setNickname(args[0]);
					if (tmp->canConnect() != 2)
						tmp->setCanConnect(0);
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
				reply = RPL_NICK(oldNick, tmp->getUsername(), tmp->getHost(), args[0]);
				(*tmp).setNickname(args[0]);
				send(tmp->getFd(), reply.c_str(), reply.length(), 0);
				if (tmp->canConnect() != 2 && !tmp->getUsername().empty()) {
					reply = RPL_WELCOME(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
					tmp->setCanConnect(2);
				}
				else
					reply = "";
			}
			if (tmp->canConnect() != 2)
				tmp->setCanConnect(1);
		}
	}
	std::cout << "RPL = " << reply << std::endl;
	send(tmp->getFd(), reply.c_str(), reply.length(), 0);
}
