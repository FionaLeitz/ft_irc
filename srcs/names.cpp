#include "../headers/irc.h"

void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the names command." << std::endl;
}

// void	names_all( Client client, std::map<std::string, Client> clients) {
// 	std::map<std::string, Client>::iterator	it = clients.begin();
// 	std::map<std::string, Client>::iterator	ite = clients.end();

// 	for ( ; it != ite; it++ ) {
// 		// envoyer au client it->first;
// 	}
// }

// void	names_channel( Client client, Channel channel ) {
// 	std::map<std::string, Client>::iterator	it = channel.getClientlist.begin();
// 	std::map<std::string, Client>::iterator	ite = channel.getClientlist.end();

// 	for ( ; it != ite; it++ ) {
// 		// envoyer au client it->first;
// 	}
// }
