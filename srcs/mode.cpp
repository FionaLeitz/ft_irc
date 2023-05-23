#include "../headers/irc.h"

void	ft_mode(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command MODE w args " << args[0] << " and " << args[1] << std::endl;
	std::string	response;
	(void)i;
	(void)fds;

	if ( args[0].empty() ) {
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), tmp->getUsername(), "MODE");
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	std::map<std::string, Channel>::iterator	it = context->channels.find(args[0]);
	if ( it == context->channels.end() ) {
		response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), args[0]);
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	Channel	*chan = &it->second;
	if ( args[1].empty() ) {
		response = ":server 324 " + (*tmp).getNickname() + " " + args[0] + " +" + chan->getMode() + "\r\n";
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return ;
	}
	int	plus[5];
	int	minus[5];
	char sign = '+';
	// ordre i t k o l
	for ( int count = 0; args[1].c_str()[i] != '\0'; count++ ) {
		if (args[1].c_str()[i] == '+')
			sign = '+';
		else if (args[1].c_str()[i] == '-')
			sign = '-';
		else if
			// on va vraiment faire un if pour chaque lettre la ? c'est degueulasse
	}
	if ( args[1][0] == '+' ) {

		// parcourir la string pour trouver "itkol" et les ajouter avec setMode(recreer une string entiere)
	}
	else if ( args[1][0] == '-' ) {
		// parcourir la string pour trouver "itkol" et les retirer avec setMode(recreer une string entiere)
	}
	// message de reponse
}

// si besoin d'arguments, voir les args suivants (dans le meme ordre que la string)

// · i: Set/remove Invite-only channel
//		si join un chan en invite only sans invite, ERR_INVITEONLYCHAN (473)

// · t: Set/remove the restrictions of the TOPIC command to channel operators
//		besoin d'etre operator pour changer le TOPIC (commande TOPIC)

// · k: Set/remove the channel key (password)
//		besoin d'un mot de passe pour join le chan (pass en argument)
//		si mauvais mot de passe pour join, ERR_BADCHANNELKEY (475)

// · o: Give/take channel operator privilege
//		donne les droits d'operator a un autre client du chan (nickname en argument)

// · l: Set/remove the user limit to channel
// 		argument, un nombre max de gens sur ce changer
// 		si join un chan qui n'a plus de place, "Cannot join #e (User limit reached)"
//		ERR_CHANNELISFULL (471)