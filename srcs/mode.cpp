#include "../headers/irc.h"

int	check_args( t_context *context, Client *tmp, std::vector<std::string> args, Channel	**chan ) {
	std::string	response;

	// if ( args[0].empty() ) {
	if ( args.size() == 0 ) {
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), tmp->getUsername(), "MODE");
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	std::map<std::string, Channel>::iterator	it = context->channels.find(args[0]);
	if ( it == context->channels.end() ) {
		response = ERR_NOSUCHCHANNEL(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), args[0]);
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	*chan = &it->second;
	std::cout << "ICI : " << args.size() << std::endl;
	// if ( args[1].empty() ) {
	if ( args.size() == 1 || args[1].empty() ) {
		response = RPL_CHANNELMODEIS((*tmp).getNickname(), args[0], " +", (*chan)->getMode());
		std::cout << response << std::endl;
		send(tmp->getFd(), response.c_str(), response.length(), 0);
		return -1;
	}
	return 0;
}

void	pass_size_operator( std::string cpy, std::string *new_arg0, std::string *new_arg1, std::string *new_arg2 ) {
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
	cpy = cpy.substr(new_arg1->size());
	save = cpy.find_first_not_of( " \r\n" );
	if (save == -1)
		save = cpy.size();
	cpy = cpy.substr(save);
	save = cpy.find_first_of(" \r\n");
	if (save == -1)
		save = cpy.size();
	*new_arg2 = cpy.substr(0, save);
	// std::cout << "premier argument : " << *new_arg0 << ", deuxieme argument : " << *new_arg1 << ", troisieme argument : " << *new_arg2 << std::endl;
	return ;
}

void	verify_valid_pass_size_operator( Client *tmp, int *letters_int, std::string *new_args, Channel **chan, int pass, int size, int oper ) {
	if ( letters_int[1] != 0 ) {
		if ( new_args[pass].size() == 0 ) {
			// std::string response = ERR_INVALIDMODEPARAM(tmp->getNickname(), tmp->getUsername(), chan[0]->getName());
			std::string response = ERR_NEEDMOREPARAMS_MODE(tmp->getNickname(), chan[0]->getName(), "k", "key", "key");
			send(tmp->getFd(), response.c_str(), response.length(), 0);
			letters_int[1] = -1;
		}
		else
			(*chan)->setPassword(new_args[pass]);
	}
	if ( letters_int[4] != 0 ) {
		if ( new_args[oper].size() == 0 ) {
			// #e o * :You must specify a parameter for the op mode. Syntax: <nick>.
			std::string response = ERR_NEEDMOREPARAMS_MODE(tmp->getNickname(), chan[0]->getName(), "o", "op", "nick");
			send(tmp->getFd(), response.c_str(), response.length(), 0);

			letters_int[4] = -1;
		}
		else
		{
			// std::cout << "Il va falloir faire quelque que chose pour definir les operators des chans." << std::endl;
			chan[0]->add_operator(new_args[oper]);
		}
	}
	if ( letters_int[2] == 1 ) {
		if ( new_args[size].size() == 0 ) {
			// #e l * :You must specify a parameter for the limit mode. Syntax: <limit>.
			std::string response = ERR_NEEDMOREPARAMS_MODE(tmp->getNickname(), chan[0]->getName(), "l", "limit", "limit");
			send(tmp->getFd(), response.c_str(), response.length(), 0);
			letters_int[2] = -1;
		}
		else if (parse_number(new_args[size].c_str()) == -1) {
			// std::cout << "La size n'est pas un nombre..." << std::endl;
			// #e l rew :Invalid limit mode parameter. Syntax: <limit>
			std::string response = ERR_INVALIDMODEPARAM(tmp->getNickname(), tmp->getUsername(), chan[0]->getName(), "limit");
			send(tmp->getFd(), response.c_str(), response.length(), 0);
			letters_int[2] = -1;
		}
		else
			(*chan)->setSizemax(atoi(new_args[size].c_str()));
	}
}

void	ft_mode(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::cout << "Received command MODE" << std::endl;// w args " << args[0] << " and " << args[1] << " and " << args[2] << " and " << args[3] << std::endl;
	Channel	*chan;
	(void)i;
	(void)fds;

	if ( check_args( context, tmp, args, &chan ) == -1 )
		return ;
	if (context->channels[args[0]].isUserOperator(*tmp) == false)
		return ;

	char		letters_char[5] = {'i','k','l','t','o'};
	int			letters_int[5];
	letters_int[0] = args[1].rfind( 'i' );
	letters_int[1] = args[1].rfind( 'k' );	// password
	letters_int[2] = args[1].rfind( 'l' );	// number
	letters_int[3] = args[1].rfind( 't' );
	letters_int[4] = args[1].rfind( 'o' );	// j'ai oublie ca !

	std::string	new_args[3];
	int	pass = 0;
	int	size = 0;
	int	oper = 0;

	if (letters_int[1] > letters_int[2] && letters_int[2] != -1 )
		pass++;
	else if (letters_int[2] != -1 && letters_int[1] != -1)
		size++;
	if (letters_int[4] > letters_int[1] && letters_int[1] != -1 )
		oper++;
	else if (letters_int[1] != -1  && letters_int[4] != -1)
		pass++;
	if (letters_int[2] > letters_int[4] && letters_int[4] != -1 )
		size++;
	else if (letters_int[4] != -1 && letters_int[2] != -1)
		oper++;

	pass_size_operator( tmp->getBuffer().substr(tmp->getBuffer().find(args[1]) + args[1].size() + 1), &new_args[0], &new_args[1], &new_args[2] );
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
	verify_valid_pass_size_operator(tmp, letters_int, new_args, &chan, pass, size, oper );

	std::string	flags = chan->getMode();
	std::string	changes;
	for (int count = 0; count < 4; count++) {
		std::string	response;
		if (letters_int[count] == 1 && flags.find(letters_char[count]) == flags.npos) {
			flags += letters_char[count];
			changes += "+";
			changes += letters_char[count];
		}
		else if (letters_int[count] == -1 && flags.find(letters_char[count]) != flags.npos) {
			flags.erase(flags.find(letters_char[count]), 1);
			changes += "-";
			changes += letters_char[count];
		}
	}
	chan->setMode( flags );
	std::string response = RPL_MODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), chan->getName(), changes); // remplacer args[1] par ce qui a ete effectivement change
																									   // par exemple, si le mode etait deja t, et que le user fait +it, on ne doit mettre que +i ici
	chan->sendToAll(response);

	// std::string	response = RPL_CHANNELMODEIS(tmp->getNickname(), args[0], " +", chan->getMode());
	// send(tmp->getFd(), response.c_str(), response.length(), 0);
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
