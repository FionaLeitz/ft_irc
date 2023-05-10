#include "headers/irc.h"

int	main(void)
{
	std::string	s = "NICK yoyo\r\nUSER yoyo 0 * :realname\r\n";
	int	ret;

	ret = s.find("USER");
	if(ret)
	{
		std::cout << "\tNickname = " << s.substr(s.find("NICK") + 5, s.find("\r\n") - 5)
		<< "\n\tUsername = " << s.substr(ret + 5, s.length() - ret - 7) << std::endl;
	}
	return 0;
}