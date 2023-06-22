#include "../headers/irc.h"

void	ft_welcome(Client *tmp, struct pollfd *fds, int i)
{
	std::string line;
	std::ifstream input_file("./banner.txt");
	(void)tmp;
	if (!input_file.is_open())
	{
		std::cout << strerror(errno) << std::endl;
        return ;
	}
    while (getline(input_file, line))
    {
        std::string message = line + "\r\n";
       	send(fds[i].fd, message.c_str(), message.length(), 0);
    }
}

void	ft_user(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	(void)context;
	std::string	reply;


	if (tmp->getPassBool() == false)
	{
		reply = ERR_UNKOWNERROR((std::string)"nickname", (std::string)"username", tmp->getHost(), args[0], (std::string)"You need to set the password first");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		return ;
	}
	if (args.size() == 0) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "USER");
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if ( (*tmp).getUsername() != "\0" && tmp->canConnect() == 2) {
		reply = ERR_ALREADYREGISTERED(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		std::cout << reply << std::endl;
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	tmp->setUsername(args[0]);
	std::cout << "dans USER : TMP->CANCONNECT = " << tmp->canConnect() << std::endl;
	if (tmp->canConnect() != 1)
		return ;
	reply = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost());
	send(fds[i].fd, reply.c_str(), reply.length(), 0);
	tmp->setCanConnect(2);
	ft_welcome(tmp, fds, i);
}
