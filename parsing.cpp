#include "headers/irc.h"

int	main(void)
{
	std::string	s = "NICK yoyo\r\nUSER yoyo 0 * :realname\r\n";
	int	ret;

	ret = s.find("\r\n");
	while (ret != -1)
	{
		std::cout << "ret = " << ret << std::endl;
		std::cout << s.find_last_of("\r\n", 0, ret) << " " << ret - 1 << std::endl;
		std::cout << s.find_last_of("\r\n", 0, 8) << std::endl;
		std::cout << s.find_last_of("\r\n", 0, 33) << std::endl;
		ret = s.find("\r\n", ret + 2);
	}
	return 0;
}