#include "../headers/irc.h"

int	check_args( t_context *context, Client *tmp, std::string *args, Channel	**chan ) {
	std::string	response;

	if ( args[0].empty() ) {
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), tmp->getUsername(), "MODE");
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	std::map<std::string, Channel>::iterator	it = context->channels.find(args[0]);
	if ( it == context->channels.end() ) {
		response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), args[0]);
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	*chan = &it->second;
	if ( args[1].empty() ) {
		response = ":server 324 " + (*tmp).getNickname() + " " + args[0] + " +" + (*chan)->getMode() + "\r\n";
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	return 0;
}

void	pass_and_size( std::string cpy, std::string *new_arg0, std::string *new_arg1 ) {
	int	save = cpy.find_first_not_of( " \r\n" );
	if (save == -1)
		save = cpy.size();
	cpy.substr(save);
	save = cpy.find_first_of( " \r\n" );
	if (save == -1)
		save = cpy.size();
	*new_arg0 = cpy.substr(0, save);
	cpy = cpy.substr(new_arg0->size());
	save = cpy.find_first_not_of( " \r\n" );
	if (save == -1)
		save = cpy.size();
	cpy = cpy.substr(save);
	save = cpy.find_first_of(" \r\n");
	if (save == -1)
		save = cpy.size();
	*new_arg1 = cpy.substr(0, save);
	return ;
}

void	verify_valid_pass_and_size( int *letters_int, std::string *new_args, Channel **chan, int pass, int size ) {
	if ( letters_int[1] != 0 ) {
		if ( new_args[pass].size() == 0 ) {
			std::cout << "Il manque le mot de passe..." << std::endl;
			// #e k * :You must specify a parameter for the key mode. Syntax: <key>.
			letters_int[1] = -1;
		}
		else
			(*chan)->setPassword(new_args[pass]);
	}
	if ( letters_int[2] == 1 ) {
		if ( new_args[size].size() == 0 ) {
			std::cout << "Il manque la size max..." << std::endl;
			// #e l * :You must specify a parameter for the limit mode. Syntax: <limit>.
			letters_int[2] = -1;
		}
		else if (parse_number(new_args[size].c_str()) == -1) {
			std::cout << "La size n'est pas un nombre..." << std::endl;
			// #e l rew :Invalid limit mode parameter. Syntax: <limit>
			letters_int[2] = -1;
		}
		else
			(*chan)->setSizemax(atoi(new_args[size].c_str()));
	}
}

void	ft_mode(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::cout << "Received command MODE w args " << args[0] << " and " << args[1] << " and " << args[2] << " and " << args[3] << std::endl;
	Channel	*chan;
	(void)i;
	(void)fds;

	if ( check_args( context, tmp, args, &chan ) == -1 )
		return ;

	char		letters_char[5] = {'i','k','l','o','t'};
	int			letters_int[5];
	std::string	new_args[2];
	letters_int[0] = args[1].rfind( 'i' );
	letters_int[1] = args[1].rfind( 'k' );	// password
	letters_int[2] = args[1].rfind( 'l' );	// number
	letters_int[3] = args[1].rfind( 'o' );
	letters_int[4] = args[1].rfind( 't' );

	int	pass = letters_int[1] > letters_int[2];
	int	size = letters_int[1] < letters_int[2];
	
	pass_and_size( tmp->getBuffer().substr(tmp->getBuffer().find(args[1]) + args[1].size() + 1), &new_args[0], &new_args[1]);

	for (int count = 0; count < 5; count++) {
		int	plus = args[1].rfind('+');
		int minus = args[1].rfind('-');
		if (letters_int[count] == -1)
			letters_int[count] = 0;
		else {
			while (1) {
				if ( letters_int[count] > plus && plus >= minus ) {
					letters_int[count] = 1;
					break ;
				}
				else if ( letters_int[count] > minus && minus > plus ) {
					letters_int[count] = -1;
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
	verify_valid_pass_and_size( letters_int, new_args, &chan, pass, size );

	std::string	flags = chan->getMode();
	for (int count = 0; count < 5; count++) {
		if (letters_int[count] == 1 && flags.find(letters_char[count]) == flags.npos) {
			flags += letters_char[count];
			// titi sets mode +t on #e ==> un envoi pour chaque nouvelle lettre
		}
		else if (letters_int[count] == -1 && flags.find(letters_char[count]) != flags.npos) {
			flags.erase(flags.find(letters_char[count]), 1);
			// titi sets mode -t on #e ==>un envoi pour chaque nouvelle lettre
		}
	}
	std::cout << "LES NOUVEAUX FLAGS : " << flags << std::endl;
	chan->setMode( flags );

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