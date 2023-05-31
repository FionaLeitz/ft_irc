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

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args) {
	(void)context;
	(void)fds;
	(void)i;
	(void)args;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the oper command." << std::endl;
}

// void	oper( Client client, /*liste de channels passer en argument*/) {
// 	// utiliser suppress_client( client.getNickname() ) sur chaque channel
// 	// utiliser suppress_operator( client.getNickname() ) sur chaque channel ??? Je sais plus pourquoi j'ai marque ca
// }
