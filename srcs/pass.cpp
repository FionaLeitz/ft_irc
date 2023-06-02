#include "../headers/irc.h"

void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command PASS" << std::endl;// w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;
	(void)tmp;
	(void)fds;
	(void)i;

	if (atoi(args[0].c_str()) == context->password)
	{
		std::cout << "Password is valid" << std::endl;
		// response = RPL_WELCOME((*tmp).getNickname(), (*tmp).getUsername());
		// send(fds[i].fd, response.c_str(), response.length(), 0);
	}
	else
	{
		std::cout << "Invalid pass" << std::endl;
		response = ERR_PASSWDMISMATCH(tmp->getNickname());
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		//la il faut trouver un moyen de refuser sa connexion ensuite
	}
}