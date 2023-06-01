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

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;

	std::string							response;
	std::vector<std::string>			op_hosts;
	std::string							clientHost;
	std::vector<std::string>::iterator	it;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the oper command." << std::endl;
	// if (args[0].size() == 0 || args[1].size() == 0)
	if ( args.size() < 2)
	{
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "OPER");
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		return ;
	}
	// - if wrong password for the given name --> ERR_PASSWDMISMATCH
	std::cout << "Trying to log as operator with name: " << args[0] << ", password: " << args[1] << ", and host: " << tmp->getHost() << std::endl;

	 // - if wrong host for the given name --> ERR_NOOPERHOST
	op_hosts = ft_split(context->op_host, " ");
	clientHost = tmp->getHost().substr(0, tmp->getHost().find(':'));
	for (it = op_hosts.begin(); it != op_hosts.end(); ++it) {
		if (clientHost == *it)
		{
			std::cout << "host ok tu peux devenir oprateur bravo" << std::endl;
			break ;
		}
	}
	if (it == op_hosts.end())
			std::cout << "mauvais host !!!! dsl tu ne peux pas devenir operateur" << std::endl;


// - if valid name and valid password and valid host --> RPL_YOUREOPER +  MODE message indicating their new user mode

}

// void	oper( Client client, /*liste de channels passer en argument*/) {
// 	// utiliser suppress_client( client.getNickname() ) sur chaque channel
// 	// utiliser suppress_operator( client.getNickname() ) sur chaque channel ??? Je sais plus pourquoi j'ai marque ca
// }
