#include "../headers/irc.h"

void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command PASS" << std::endl;
	std::string	response;

	if (args.size() == 0) {
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "USER");
		send(fds[i].fd, response.c_str(), response.length(), 0);
		return ;
	}
	if (args[0] == context->password)
	{
		std::cout << "Password is valid" << std::endl;
		tmp->setPassBool(true);
		// response = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername());
		// send(fds[i].fd, response.c_str(), response.length(), 0);
	}
	else
	{
		std::cout << "Invalid pass" << std::endl;
		tmp->setPassBool(false);
		response = ERR_PASSWDMISMATCH(tmp->getNickname());
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		response.clear();
		close( fds[i].fd );
		std::map<int, Client>::iterator it = context->clients.find(fds[i].fd);
		context->clients.erase(it);
		fds[i].fd = -1;
	}
}