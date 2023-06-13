#include "../headers/irc.h"

std::vector<std::string>	ft_split( std::string to_split, std::string separate ) {
	std::vector<std::string>	my_list;
	size_t					save = 0;
	size_t					tmp = save;

	while ( save != std::string::npos ) {
		save = to_split.find_first_of( separate, save );
		if ( save != std::string::npos )
			save++;
		// if ( save == 0 )
		// 	save++;
		std::string	to_push = to_split.substr( tmp, save - tmp - 1 );
		tmp = save;
		my_list.push_back( to_push );
	}

	return my_list;
}

int	isAuthorized(const Client &user, std::string command)
{
	std::string reply;

	if (user.canConnect() == false)
	{
		reply = ERR_UNKOWNERROR(user.getNickname(), user.getUsername(), user.getHost(), command, "You must set a nickname first.");
		send(user.getFd(), reply.c_str(), reply.size(), 0);
		return 1;
	}
	return 0;
}

// int	main( int argc, char **argv ) {
// 	if ( argc != 3 )
// 		return 1;
// 	std::vector<std::string>	test = ft_split( argv[1], argv[2] );

// 	std::vector<std::string>::iterator	it = test.begin();
// 	std::vector<std::string>::iterator	ite = test.end();
// 	for ( ; it != ite; it++ )
// 		std::cout << (*it) << std::endl;

// 	return 0;
// }