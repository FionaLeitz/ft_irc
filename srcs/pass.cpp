#include "../headers/irc.h"

void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command PASS" << std::endl;// w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;
	// (void)tmp;
	(void)fds;
	// (void)i;

	std::cout << "args[0] = " << args[0] << " et context->password = " << context->password << std::endl;
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
		//la il faut trouver un moyen de refuser sa connexion ensuite
		response = ERR_PASSWDMISMATCH(tmp->getNickname());
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		response.clear();
		// response = tmp->getNickname();
		// response += " ERROR: refused connexion\r\n";
		// send(tmp->getFd(), response.c_str(), response.length(), 0);
		// context->clients.erase()
		close( fds[i].fd );
		fds[i].fd = -1;
	}
}