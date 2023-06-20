#include "../headers/irc.h"

void	ft_list(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the list command." << std::endl;
	std::string response = RPL_LISTSTART(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
	send(tmp->getFd(), response.c_str(), response.length(), 0);
	for ( std::map<std::string,Channel>::iterator it = context->channels.begin(); it != context->channels.end(); it++ ) {
		response.clear(); std::ostringstream oss;
		oss << it->second.getClientlist().size();
		response = RPL_LIST(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), it->first, oss.str(), it->second.getTopic());
		send(tmp->getFd(), response.c_str(), response.length(), 0);
	}
	response.clear();
	response = RPL_LISTEND(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
	send(tmp->getFd(), response.c_str(), response.length(), 0);
}
