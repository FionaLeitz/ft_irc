#include "../headers/irc.h"

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the oper command." << std::endl;
}

// void	oper( Client client, /*liste de channels passer en argument*/) {
// 	// utiliser suppress_client( client.getNickname() ) sur chaque channel
// 	// utiliser suppress_operator( client.getNickname() ) sur chaque channel ??? Je sais plus pourquoi j'ai marque ca
// }
