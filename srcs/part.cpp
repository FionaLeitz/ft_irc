#include "../headers/irc.h"

void	ft_part(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	// (void)context;
	// (void)fds;
	// (void)i;
	// (void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the part command." << std::endl;

	int	count = 0;
	std::string	message;
	for ( std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++ ) {
		std::cout << "args["<<count<<"] = " << *it << std::endl;
		if (count != 0) {
			if (count == 1)
				message = &(*it)[1];
			else
				message += *it;
			message += " ";
		}
		count++;
	}
	// args[0] = le nom du/des channels
	// args[1] = la raison du part
	(void)fds;
	(void)i;
	std::vector<std::string>	chans = ft_split( args[0], "," );
	std::string					response;
	for ( std::vector<std::string>::iterator 			it = chans.begin(); it != chans.end(); it++ ) {
		std::map<std::string,int>::const_iterator		finding = tmp->getChannelList().find(*it);
		std::map<std::string, Channel>::iterator		existingchannel = context->channels.find( *it );
		if ( existingchannel == context->channels.end() ) {
			// message ERR_NOSUCHCHANNEL (403)
			response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), *it );
			send(tmp->getFd(), response.c_str(), response.size(), 0);
			std::cout << "Le channel " <<*it<< " n'existe pas" << std::endl;
		}
		else if ( finding == tmp->getChannelList().end() || finding->second == 0 ) {
			// message ERR_NOTONCHANNEL (442)
			response = ERR_NOTONCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), *it );
			send(tmp->getFd(), response.c_str(), response.size(), 0);
			std::cout << "Il n'est pas dans le channel "<< *it << std::endl;
		}
		else {
			std::cout << "Il peut quitter le channel "<< *it << std::endl;
			response = RPL_QUIT(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), message);
			existingchannel->second.sendToAll( response);
			// theoriquement, on envoie a tou les gens du chan qu'on quitte
			tmp->removeChannel( *it );
			existingchannel->second.suppress_client( tmp->getNickname() );
			// on remove ce channel des listes du client
			// et on le vire des listes du channel
		}
	}
}

// PROBLEME ! On envoit le message de quit pas au bon endroit, et on quit pas vraiment et ca c'est nul
// c'etait juste qu'il fallait utiliser la fonction sendToAll au lieu de sendMessage ;)
