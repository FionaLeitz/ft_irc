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
	std::string	reply;
	std::string	RPLnick;
	std::map<int, Client>::const_iterator it;
	std::string oldNick;
	std::vector<std::string> user_args;

	if (tmp->getPassBool() == false)
		return ;
	if (args.size() < 1)
		reply = ERR_NONICKNAMEGIVEN();
	else
	{
		// check caracteres interdits
		if (isStringAlnum(args[0]) == false)
		{
			reply = ERR_ERRONEUSNICKNAME(args[0]);
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
				if (args[0] == it->second.getNickname() && it->second.getFd() != tmp->getFd())
				{
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
				tmp->setCanConnect(1);
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
					ft_welcome(tmp, fds, i);
				}
				else
					reply = "";
			}
		}
	}
	if (!reply.empty())
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
}
