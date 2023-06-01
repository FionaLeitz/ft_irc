#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <cstring>

#include <fstream>  
#include <ostream>     // std::ofstream
int	main(void)
{
	std::ifstream	confFile;
	std::ofstream	newConfFile;
	std::string		swap;
	// std::string		line;
	size_t			found;
	// size_t			found_pass;
	// long			pos;

	confFile.open("./.IRCd-config"); // Ouvrir le fichier en lecture
    if (!confFile) {
        std::cout << "in update_confile : Failed to open conf file : " << strerror(errno) << std::endl;
		confFile.close();
        return 1;
    }
	std::getline(confFile, swap, (char)EOF);
	confFile.close();
	found = swap.find("operator_name =");
	found = swap.find("\n", found);
	if (found != std::string::npos)
		swap.replace(found, 1, "titato\n");
	else {
		std::cout << "Error in conf file." << std::endl;
		return 1;
	}

	found = swap.find("operator_password =");
	found = swap.find("\n", found);
	if (found != std::string::npos)
		swap.replace(found, 1, "12312312\n");
	else {
		std::cout << "Error in conf file : missing \n" << std::endl;
		return 1;
	}
	// swap.replace(found, 1, "12312312\n");
	std::cout << swap;
	newConfFile.open("./.IRCd-config");
	newConfFile << swap;
    // while (std::getline(confFile, line)) {
    //     found = line.find("operator_name =");
	// 	found_pass = line.find("operator_password =");
    //     if (found != std::string::npos)
	// 	{
	// 		swap << line;
	// 		swap << "ocucoucoucouc" << std::endl;
	// 	}
    //     else if (found_pass != std::string::npos)
	// 	{
	// 		swap << line;
	// 		swap << "123456" << std::endl;
	// 	}
	// 	else 
	// 		swap << line << std::endl;
    // }
	// confFile.close();
	// newConfFile.open("./.IRCd-config");
	// newConfFile << swap;
}