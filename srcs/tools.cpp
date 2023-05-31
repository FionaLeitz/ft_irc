#include "../headers/irc.h"

std::list<std::string>	ft_split( std::string to_split, std::string separate ) {
	std::list<std::string>	my_list;
	size_t					save = 0;
	size_t					tmp = save;

	while ( save != std::string::npos ) {
		save = to_split.find_first_of( separate, save + 1 );
		std::string	to_push = to_split.substr( tmp, save - tmp );
		tmp = save + 1;
		my_list.push_back( to_push );
	}

	return my_list;
}

int	main( int argc, char **argv ) {
	if ( argc != 3 )
		return 1;
	std::list<std::string>	test = ft_split( argv[1], argv[2] );

	std::list<std::string>::iterator	it = test.begin();
	std::list<std::string>::iterator	ite = test.end();
	for ( ; it != ite; it++ )
		std::cout << (*it) << std::endl;

	return 0;
}