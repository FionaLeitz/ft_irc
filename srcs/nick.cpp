#include "../headers/irc.h"

void	nick( Client client, std::string new_nick ) {
	client.setNickname( new_nick );
	// est ce qu'on peut changer une key dans une map ? Si oui, changer la string associee
	// au client dans la map de clients
	// si non, supprimer le client de la map, puis le rajouter avec son nouveau nick
	// devoir changer dans tous les channels ? Pas plus simple que ca ?
}
