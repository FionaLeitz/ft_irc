#include "../headers/irc.h"

void	ft_invite(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)fds;
	(void)i;

	std::string response;
	std::cout << "Client "<<tmp->getNickname() << " is trying to use the invite command w args " << args[0] <<  " and " << args[1] << std::endl;
	// INVITE tutu #u
	//if user existe pas ?

	response = RPL_INVITE_MSG(tmp->getNickname(), tmp->getUsername(), args[0], args[1]);
	std::cout << response << std::endl;
	int fd = findClientFd(context->clients, args[0]);
	int ret = send(fd, response.c_str(), response.size(), 0);
	std::cout << " ret = " << ret << " errno: " << errno << " " << response << std::endl;
	if (ret == -1 && errno == 9)
		response = ERR_NOSUCHNICK(tmp->getNickname(), tmp->getUsername(), args[0]);
	else
		response = RPL_INVITING(tmp->getNickname(), args[1], args[0]);
	send(tmp->getFd(), response.c_str(), response.size(), 0);
}
