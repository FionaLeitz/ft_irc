#include "../headers/irc.h"

std::string	concat( std::vector<std::string> reason ) {
	std::string msg;
	std::vector<std::string>::iterator	it = reason.begin();
	it++;

	for ( ; it != reason.end(); it++ ) {
		msg.append(*it);
		msg.append(" ");
	}
	msg = &msg[1];
	return msg;
}

std::string killMessage(std::string killer, std::string reason)
{
	std::string msg;

	msg = "Killed (";
	msg.append(killer);
	msg.append(": ");
	msg.append( reason );
	msg.append(")");
	return msg;
}

void	ft_kill(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	std::string	reply;
	int			fd;
	Client		*target;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the kill command." << std::endl;
	if (tmp->getOperator() == false)
	{
		reply = ERR_NOPRIVILEGES(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		return ;
	}
	if (args.size() < 2)
	{
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "KILL");
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
	}
	else
	{
		fd = findClientFd(context->clients, args[0]);
		if (fd == -1) {
			reply = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "", args[0]);
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		}
		else
		{
			target = &context->clients[fd];
			std::string	reason = concat( args );
			reply = RPL_KILL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], reason);
			std::cout << "RPL = " << reply << std::endl;
			send(fd, reply.c_str(), reply.size(), 0);
			reply = RPL_QUIT(target->getNickname(), target->getUsername(), target->getHost(), killMessage(tmp->getNickname(), reason));
			context->clients[fd].leaveAllChannels(context, reply);
			reply = RPL_CLOSINGLINK(tmp->getNickname(), reason);
			std::cout << "RPL = " << reply << std::endl;
			send(fd, reply.c_str(), reply.size(), 0);
			//close la connexion
			context->clients.erase(fd);
			close(fd);
			for (int i = 0; i < context->socket_nbr[0]; i++) {
				if (fds[i].fd == fd)
				{
					fds[i].fd = -1;
					break ;
				}
			}
		}
	}
}
