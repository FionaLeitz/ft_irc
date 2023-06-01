#include "../headers/irc.h"

void	ft_restart(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the restart command." << std::endl;
}
