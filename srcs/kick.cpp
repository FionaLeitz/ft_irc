#include "../headers/irc.h"

void	kick( Client operator, std::string nick, Channel channel ) {
	// verifier que l'operateur (le kickeur) ait les droits pour le faire
	if ( channel.getOperators().find( operator.getNickname() ) == channel.getOperators().end() )
		return ;
	channel.suppress_client( nick );
}
