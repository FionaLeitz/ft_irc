#include "../headers/irc.h"

void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	reply;

	if (args.size() == 0) {
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "PASS");
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (tmp->canConnect() == 2 && tmp->getPassBool() == true)
		return ;
	if (args[0] == context->password)
	{
		tmp->setPassBool(true);
	}
	else
	{
		tmp->setPassBool(false);
		reply = ERR_PASSWDMISMATCH((std::string)"nickname");
		send(tmp->getFd(), reply.c_str(), reply.length(), 0);
		if (fds[i].fd) {
			close( fds[i].fd );
			fds[i].fd = -1;
		}
		std::map<int, Client>::iterator it = context->clients.find(fds[i].fd);
		if (it != context->clients.end())
			context->clients.erase(it);
		fds[i].fd = -1;
	}
}