#include "../headers/irc.h"

void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command PASS" << std::endl;
	std::string	reply;

	if (args.size() == 0) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "USER");
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (args[0] == context->password)
	{
		std::cout << "Password is valid" << std::endl;
		tmp->setPassBool(true);
		// reply = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername());
		// send(fds[i].fd, reply.c_str(), reply.length(), 0);
	}
	else
	{
		std::cout << "Invalid pass" << std::endl;
		tmp->setPassBool(false);
		reply = ERR_PASSWDMISMATCH((std::string)"nickname");
		std::cout << reply << std::endl;
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		reply.clear();
		close( fds[i].fd );
		std::map<int, Client>::iterator it = context->clients.find(fds[i].fd);
		context->clients.erase(it);
		fds[i].fd = -1;
	}
}