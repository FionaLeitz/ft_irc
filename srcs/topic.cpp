#include "../headers/irc.h"

void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the topic command." << std::endl;
}
