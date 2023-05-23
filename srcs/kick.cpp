#include "../headers/irc.h"

void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << "is trying to use the kick command." << std::endl;
}

// void	kick( Client operator, std::string nick, Channel channel ) {
// 	// verifier que l'operateur (le kickeur) ait les droits pour le faire
// 	if ( channel.getOperators().find( operator.getNickname() ) == channel.getOperators().end() )
// 		return ;
// 	channel.suppress_client( nick );
// }
