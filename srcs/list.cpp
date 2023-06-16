#include "../headers/irc.h"

void	ft_list(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	// (void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the list command." << std::endl;
	std::string response = RPL_LISTSTART(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
	send(tmp->getFd(), response.c_str(), response.length(), 0);
	for ( std::map<std::string,Channel>::iterator it = context->channels.begin(); it != context->channels.end(); it++ ) {
		response.clear();
		// response = RPL_LIST(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), it->first, it->second.getClientlist().size(), it->second.getTopic());
		response = RPL_LIST(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), it->first, "8", it->second.getTopic());
		send(tmp->getFd(), response.c_str(), response.length(), 0);
	}
	response.clear();
	response = RPL_LISTEND(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
	send(tmp->getFd(), response.c_str(), response.length(), 0);
}

// RPL_LISTSTART(nickname, username, host) (":server 321 " + USER_ID(nickname, username, host) + " Channel :Users Name\r\n")
// RPL_LIST(nickname, username, host, channel, count, topic) (":server 322 " + USER_ID(nickname, username, host) + " " + channel + " " + count + " :" + topic "\r\n")
// RPL_LISTEND(nickname, username, host) (":server 323 " + USER_ID(nickname, username, host) + " :End of /LIST\r\n")


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