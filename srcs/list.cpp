#include "../headers/irc.h"

void	ft_list(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the list command." << std::endl;
}

// void	list_channel( Client client, std::map<std::string, Channel> channels ) {
// 	// envoyer une liste de tous les channels deja existants
// 	std::map<std::string, Channel>::iterator	it = channels.begin();
// 	std::map<std::string, Channel>::iterator	ite = channels.end();

// 	for ( ; it != ite; it++ ) {
// 		// envoyer au client it->first;
// 	}

// }

// void	list_topic( Client client, /*la liste passee en argument,*/ std::map<std::string, Channel> channels) {
// 	// envoyer une liste de tous les topics des channels demandes
// 	// est ce qu'on peut avoir plusieurs topics pour un channel ?
// }