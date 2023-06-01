#include "../headers/irc.h"

/*

The OPER command is used by a normal user to obtain IRC operator privileges.
Both parameters are required for the command to be successful.
- if != 2 params --> ERR_NEEDMOREPARAMS (461)
- if wrong password for the given name --> ERR_PASSWDMISMATCH
- if wrong host for the given name --> ERR_NOOPERHOST
- if valid name and valid password and valid host --> RPL_YOUREOPER +  MODE message indicating their new user mode
    ERR_PASSWDMISMATCH (464)
    ERR_NOOPERHOST (491)
    RPL_YOUREOPER (381)

The <name> specified by this command is separate to the accounts specified by SASL authentication, and is generally stored in the
IRCd configuration.
*/

int	update_confFile(std::string op_newName, std::string op_newPassword)
{
	std::ifstream	confFile;
	std::ofstream	newConfFile;
	std::string		swap;
	size_t			found;

	confFile.open("./.IRCd-config");
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
		swap.replace(found, 1, op_newName.append("\n"));
	else {
		std::cout << "Error in conf file." << std::endl;
		return 1;
	}
	found = swap.find("operator_password =");
	found = swap.find("\n", found);
	if (found != std::string::npos)
		swap.replace(found, 1, op_newPassword.append("\n"));
	else {
		std::cout << "Error in conf file : missing \n" << std::endl;
		newConfFile.close();
		return 1;
	}
	std::cout << swap;
	newConfFile.open("./.IRCd-config");
	newConfFile << swap;
	newConfFile.close();
	return 0;
}

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;

	std::string							response;
	std::vector<std::string>			op_hosts;
	std::string							clientHost;
	std::vector<std::string>::iterator	it;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the oper command." << std::endl;
	if ( args.size() < 2)
	{
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "OPER");
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		return ;
	}
	std::cout << "Trying to log as operator with name: " << args[0] << ", password: " << args[1] << ", and host: " << tmp->getHost() << std::endl;

	if (context->op_name.empty())		// si aucun operateur n'a encore ete set, il faut etre en localhost pour le devenir
	{
		op_hosts = ft_split(context->op_host, " ");
		clientHost = tmp->getHost().substr(0, tmp->getHost().find(':'));
		for (it = op_hosts.begin(); it != op_hosts.end(); ++it) {
			if (clientHost == *it)
				break ;
		}
		if (it == op_hosts.end()) {
				std::cout << "mauvais host !!!! dsl tu ne peux pas devenir operateur" << std::endl;
				response = ERR_NOOPERHOST(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
				send(tmp->getFd(), response.c_str(), response.size(), 0);
				return ;
		}
		else {
			std::cout << "host ok tu peux devenir oprateur bravo" << std::endl;
			if (update_confFile(args[0], args[1]) == 1)
				return ; // send ERR_ custom ?
			
		}
	
	}
	else		// sinon, l'operateur peut se connecter depuis n'importe quelle adresse
	{
		// - if wrong password for the given name --> ERR_PASSWDMISMATCH
	}
	


// - if valid name and valid password and valid host --> RPL_YOUREOPER +  MODE message indicating their new user mode

}

// void	oper( Client client, /*liste de channels passer en argument*/) {
// 	// utiliser suppress_client( client.getNickname() ) sur chaque channel
// 	// utiliser suppress_operator( client.getNickname() ) sur chaque channel ??? Je sais plus pourquoi j'ai marque ca
// }
