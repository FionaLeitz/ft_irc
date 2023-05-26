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
	// iklot
	int	letters[5];
	
	letters[0] = args[1].rfind( 'i' );
	letters[1] = args[1].rfind( 'k' );
	letters[2] = args[1].rfind( 'l' );
	letters[3] = args[1].rfind( 'o' );
	letters[4] = args[1].rfind( 't' );
	
	for (int count = 0; count < 5; count++) {
		int	plus = args[1].rfind('+');
		int minus = args[1].rfind('-');
		if (letters[count] == -1)
			letters[count] = 0;
		else {
			while (1) {
				if ( letters[count] > plus && plus >= minus ) {
					letters[count] = 1;
					break ;
				}
				else if ( letters[count] > minus && minus > plus ) {
					letters[count] = -1;
					break ;
				}
				else {
					if (plus != -1)
						plus = args[1].rfind('+', plus - 1);
					if (minus != -1)
						minus = args[1].rfind('-', minus - 1);
				}
			}
		}
	}
	std::cout << "FLAGS ? i=" << letters[0]<<" k="<<letters[1]<<" l="<<letters[2]<<" o="<<letters[3]<<" t="<<letters[4]<< std::endl;
	std::string	flags = chan->getMode();
	if (letters[0] == 1 && flags.find('i') == flags.npos)
		flags += 'i';
	else if (letters[0] == -1 && flags.find('i') != flags.npos)
		flags.erase(flags.find('i'), 1);
	if (letters[1] == 1 && flags.find('k') == flags.npos)
		flags += 'k';
	else if (letters[1] == -1 && flags.find('k') != flags.npos)
		flags.erase(flags.find('k'), 1);
	if (letters[2] == 1 && flags.find('l') == flags.npos)
		flags += 'l';
	else if (letters[2] == -1 && flags.find('l') != flags.npos)
		flags.erase(flags.find('l'), 1);
	if (letters[3] == 1 && flags.find('o') == flags.npos)
		flags += 'o';
	else if (letters[3] == -1 && flags.find('o') != flags.npos)
		flags.erase(flags.find('o'), 1);
	if (letters[4] == 1 && flags.find('t') == flags.npos)
		flags += 't';
	else if (letters[4] == -1 && flags.find('t') != flags.npos)
		flags.erase(flags.find('t'), 1);
	std::cout << "LES NOUVEAUX FLAGS : " << flags << std::endl;
	// for (int count = 0; count < 5; count++) {
	// 	if (letters[count] == 1)
	// 		flags +=
	// }

	// std::cout << "letters numbers = " << letters[0] << " "<< letters[1] << " "<< letters[2] << " "<< letters[3] << " "<< letters[4] << " "<<std::endl;

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