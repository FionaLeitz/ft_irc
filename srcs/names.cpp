#include "../headers/irc.h"

void	names_all( Client client, std::map<std::string, Client> clients) {
	std::map<std::string, Client>::iterator	it = clients.begin();
	std::map<std::string, Client>::iterator	ite = clients.end();

	for ( ; it != ite; it++ ) {
		// envoyer au client it->first;
	}
}

void	names_channel( Client client, Channel channel ) {
	std::map<std::string, Client>::iterator	it = channel.getClientlist.begin();
	std::map<std::string, Client>::iterator	ite = channel.getClientlist.end();

	for ( ; it != ite; it++ ) {
		// envoyer au client it->first;
	}
}
