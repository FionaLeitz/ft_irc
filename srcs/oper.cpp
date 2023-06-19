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
   

The <name> specified by this command is separate to the accounts specified by SASL authentication, and is generally stored in the
IRCd configuration.
*/

/*
Si le host est mauvais, ca degage !
Si le host est bon :
	- si le name est nouveau, on le rajoute avec le mot de passe
	- si le name existe, on verifie le mot de pass correspondant
		- si c'est bon, c'est cool
		- si c'est pas bon, ca degage
Est ce qu'on le reset quand on relance le serveur, ou est ce qu'on le garde ?
	- je dirais qu'on le reset, mais pas sure...
	- option : dans le Makefile on ajoute une commande qui le reset ! Comme ca on choisit si on le remet a zero ou pas
*/

// int	update_confFile(std::string op_newName, std::string op_newPassword)
// {
// 	std::ifstream	confFile;
// 	std::ofstream	newConfFile;
// 	// std::string		swap;
// 	std::string		lines;
// 	// size_t			found;

// 	confFile.open("./.IRCd-config");
//     if (!confFile) {
//         std::cout << "in update_confile : Failed to open conf file : " << strerror(errno) << std::endl;
// 		confFile.close();
//         return 1;
//     }
// 	std::getline(confFile, lines);
// 	std::string	ip = &lines[16];
// 	int	end = ip.find(' ');
// 	if (end != std::string::npos)
// 		ip.resize(end);
// 	std::cout << "ip = --" << ip <<"--" << std::endl;
// 	// std::getline(confFile, swap, (char)EOF);
// 	// confFile.close();
// 	// found = swap.find("operator_name =");
// 	// found = swap.find("\n", found);
// 	// if (found != std::string::npos)
// 	// 	swap.replace(found, 1, op_newName.append("\n"));
// 	// else {
// 	// 	std::cout << "Error in conf file." << std::endl;
// 	// 	return 1;
// 	// }
// 	// found = swap.find("operator_password =");
// 	// found = swap.find("\n", found);
// 	// if (found != std::string::npos)
// 	// 	swap.replace(found, 1, op_newPassword.append("\n"));
// 	// else {
// 	// 	std::cout << "Error in conf file : missing \n" << std::endl;
// 	// 	newConfFile.close();
// 	// 	return 1;
// 	// }
// 	// std::cout << swap;
// 	// newConfFile.open("./.IRCd-config");
// 	// newConfFile << swap;
// 	// newConfFile.close();
// 	return 0;
// }

int	isinop_names(std::string name, std::vector<std::string> op_names) {
	std::vector<std::string>::iterator	it;
	int	count = 0;
	for (it = op_names.begin(); it != op_names.end(); it++) {
		if (*it == name)
			return count;
		count++;
	}
	return -1;
}

std::string	add_in_config(std::string name, std::string pass, Client *tmp) {
	std::ifstream	confFile;
	std::ofstream	newConfFile;
	std::string		swap;
	std::string		lines;
	size_t			found;
	std::string		reply;

	if (isStringAlnum(name) == false){
		reply = ERR_ERRONEUSNICKNAME(name);
		return reply;
	}

	confFile.open("./.IRCd-config");
    if (!confFile) {
        std::cout << "in update_confile : Failed to open conf file : " << strerror(errno) << std::endl;
		confFile.close();
		reply  = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "OPER", "Failed to open conf file");
        return reply;
    }
	std::getline(confFile, lines);
	std::string	ip = &lines[16];
	unsigned long	end = ip.find(' ');
	if (end != std::string::npos)
		ip.resize(end);
	std::getline(confFile, swap, (char)EOF);
	confFile.close();
	found = swap.find("operator_name =");
	found = swap.find("\n", found);
	if (found != std::string::npos) {
		name.insert(name.begin(), ' ');
		swap.replace(found, 1, name.append("\n"));
	}
	else {
		std::cout << "Error in conf file." << std::endl;
		reply  = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "OPER", "Error in conf file");
		return reply;
	}
	found = swap.find("operator_password =");
	found = swap.find("\n", found);
	if (found != std::string::npos) {
		pass.insert(pass.begin(), ' ');
		swap.replace(found, 1, pass.append("\n"));
	}
	else {
		std::cout << "Error in conf file : missing newline" << std::endl;
		reply  = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "OPER", "Error in conf file");
		newConfFile.close();
		return reply;
	}
	std::cout << swap;
	newConfFile.open("./.IRCd-config");
	newConfFile << swap;
	newConfFile.close();
	return reply;
}

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	(void)context;

	std::string					response;
	std::ifstream				confFile;
	std::string					lines;
	std::vector<std::string>	op_names;
	std::vector<std::string>	op_pass;
	std::string					client_ip;

	std::cout << "Client "<<tmp->getNickname() << " is trying to use the oper command." << std::endl;
	if ( args.size() < 2)
	{
		response = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "OPER");
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		return ;
	}
	std::cout << "Trying to log as operator with name: " << args[0] << ", password: " << args[1] << ", and host: " << tmp->getHost() << std::endl;

	if (tmp->getOperator() == true)
	{
		response = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "OPER", "You are already logged as a network operator");
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		return ;
	}
	confFile.open("./.IRCd-config");
    if (!confFile) {
        std::cout << "in update_confile : Failed to open conf file : " << strerror(errno) << std::endl;
		confFile.close();
        return ;
    }
	std::getline(confFile, lines);
	std::string	ip = &lines[16];
	unsigned long	end = ip.find(' ');
	if (end != std::string::npos)
		ip.resize(end);
	std::getline(confFile, lines);		
	std::string	names = &lines[16];
	op_names = ft_split(names, " ");
	std::getline(confFile, lines);		
	std::string	pass = &lines[20];
	op_pass = ft_split(pass, " ");
	client_ip = tmp->getHost();
	end = client_ip.find(':');
	if (end != std::string::npos)
		client_ip.resize(end);
	int	place = isinop_names(args[0], op_names);
	std::cout <<"Place = "<<place<<std::endl;

	if ( place != -1) {
		std::cout << "Le name est dans les operator_name" << std::endl;
		if (op_pass[place] == args[1]) {
			std::cout << "Le mot de passe est le bon, il peut se connecter en tant que OPERATOR"<<std::endl;
			response = RPL_YOUREOPER(tmp->getNickname());
			send(tmp->getFd(), response.c_str(), response.size(), 0);
			response = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
			send(tmp->getFd(), response.c_str(), response.size(), 0);
			tmp->setOperator(true);
		}
		else {
			std::cout << "Le mot de passe est mauvais, il ne devient pas operator" << std::endl;
			response = ERR_PASSWDMISMATCH(tmp->getNickname());
			send(tmp->getFd(), response.c_str(), response.size(), 0);
		}
	}
	else if (client_ip == ip ) {
		std::cout << "Il est sur le bon IP, il peut creer un name et un pass pour etre OPERATOR" << std::endl;
		response = add_in_config(args[0], args[1], tmp);
		if (!response.empty()) {
			send(tmp->getFd(), response.c_str(), response.size(), 0);
			return ;
		}
		response = RPL_YOUREOPER(tmp->getNickname());
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		response = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
		send(tmp->getFd(), response.c_str(), response.size(), 0);
		tmp->setOperator(true);
	}
	else {
		std::cout << "Il est refuse ! Il ne peut pas devenir operator" << std::endl;
		response = ERR_NOOPERHOST(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		send(tmp->getFd(), response.c_str(), response.size(), 0);
	}
	confFile.close();

	// if (context->op_name.empty())		// si aucun operateur n'a encore ete set, il faut etre en localhost pour le devenir
	// {
	// 	op_hosts = ft_split(context->op_host, " ");
	// 	clientHost = tmp->getHost().substr(0, tmp->getHost().find(':'));
	// 	for (it = op_hosts.begin(); it != op_hosts.end(); ++it) {
	// 		if (clientHost == *it)
	// 			break ;
	// 	}
	// 	if (it == op_hosts.end()) {
	// 			std::cout << "mauvais host !!!! dsl tu ne peux pas devenir operateur" << std::endl;
	// 			response = ERR_NOOPERHOST(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
	// 			send(tmp->getFd(), response.c_str(), response.size(), 0);
	// 			return ;
	// 	}
	// 	else {
	// 		std::cout << "host ok tu peux devenir oprateur bravo" << std::endl;
	// 		if (update_confFile(args[0], args[1]) == 1)
	// 			return ; // send ERR_ custom ?
	// 		response = RPL_YOUREOPER(tmp->getNickname());
	// 		send(tmp->getFd(), response.c_str(), response.size(), 0);
	// 		response = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
	// 		send(tmp->getFd(), response.c_str(), response.size(), 0);
	// 		tmp->setOperator(true);
	// 	}
	
	// }
	// else							// sinon, l'operateur peut se connecter depuis n'importe quelle adresse
	// {
	// 	if (args[1] == context->op_password && args[0] == context->op_name) {
	// 		response = RPL_YOUREOPER(tmp->getNickname());
	// 		send(tmp->getFd(), response.c_str(), response.size(), 0);
	// 		response = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
	// 	}
	// 	else 
	// 		response = ERR_PASSWDMISMATCH(tmp->getNickname());
	// 	send(tmp->getFd(), response.c_str(), response.size(), 0);
	// }
	


// - if valid name and valid password and valid host --> RPL_YOUREOPER +  MODE message indicating their new user mode

}

// void	oper( Client client, /*liste de channels passer en argument*/) {
// 	// utiliser suppress_client( client.getNickname() ) sur chaque channel
// 	// utiliser suppress_operator( client.getNickname() ) sur chaque channel ??? Je sais plus pourquoi j'ai marque ca
// }
