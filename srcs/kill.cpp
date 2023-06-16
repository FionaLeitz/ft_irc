#include "../headers/irc.h"

std::string killMessage(std::string killer, std::string reason)
{
	std::string msg;

	msg = "Killed (";
	msg.append(killer);
	msg.append(": ");
	msg.append(reason);
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
		// return ;
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
			reply = RPL_KILL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0], args[1]);
			std::cout << "RPL = " << reply << std::endl;
			send(fd, reply.c_str(), reply.size(), 0);
			reply = RPL_QUIT(target->getNickname(), target->getUsername(), target->getHost(), killMessage(tmp->getNickname(), args[1]));
			context->clients[fd].leaveAllChannels(context->channels, reply);
			reply = "ERROR :Closing Link: server (Killed (Operator (Excessive spamming)))\r\n"; //test
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


// -the KILL command is used to close the connection between a given client and the server they are
// connected to
// - privileged command, only available to IRC Operators

// -      Command: KILL
//   Parameters: <nickname> <comment>

// - <nickname> : user to be ‘killed’
// - <comment> : shown to all users and to the user themselves upon being killed.

// When a KILL command is used :
	// 1. the client being killed receives the KILL message (the <source> of the message SHOULD be
	// the operator who performed the command)
	// 2. The user being killed + every user sharing a channel with them receives a QUIT message
// representing that they are leaving the network. The <reason> on this QUIT message typically has
// the form: "Killed (<killer> (<reason>))" where <killer> is the nickname of the user who performed
// the KILL.
	// 3. The user being killed then receives the ERROR message, typically containing a <reason>
// of "Closing Link: <servername> (Killed (<killer> (<reason>)))"
		// 4. After this, their connection is closed.

// If a KILL message is received by a client, it means that the user specified by <nickname> is being
// killed. With certain servers, users may elect to receive KILL messages created for other users to
// keep an eye on the network. This behavior may also be restricted to operators.

// Clients can rejoin instantly after this command is performed on them. However, it can serve as a
// warning to a user to stop their activity. As it breaks the flow of data from the user, it can
// also be used to stop large amounts of ‘flooding’ from abusive users or due to accidents.
// Abusive users may not care and promptly reconnect and resume their abusive behaviour. In these
// cases, opers may look at the KLINE command to keep them from rejoining the network for a longer
// time.

// As nicknames across an IRC network MUST be unique, if duplicates are found when servers join,
// one or both of the clients MAY be KILLed and removed from the network. Servers may also handle
// this case in alternate ways that don’t involve removing users from the network.

// Servers MAY restrict whether specific operators can remove users on other servers (remote users). If the operator tries to remove a remote user but is not privileged to, they should receive the ERR_NOPRIVS (723) numeric.

// <comment> SHOULD reflect why the KILL was performed. For user-generated KILLs, it is up to the
// user to provide an adequate reason.

// Numeric Replies:

//     ERR_NOSUCHSERVER (402)
//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOPRIVILEGES (481)
//     ERR_NOPRIVS (723)
