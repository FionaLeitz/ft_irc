#include "../headers/irc.h"

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
	confFile.);
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
	newConfFile.open("./.IRCd-config");
	newConfFile << lines << std::endl;
	newConfFile << swap;
	newConfFile.close();
	return reply;
}

void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args) {
	(void)fds;
	(void)i;
	(void)context;

	std::string					reply;
	std::ifstream				confFile;
	std::string					lines;
	std::vector<std::string>	op_names;
	std::vector<std::string>	op_pass;
	std::string					client_ip;

	if ( args.size() < 2)
	{
		reply = ERR_NEEDMOREPARAMS(tmp->getNickname(), "", "OPER");
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		return ;
	}

	if (tmp->getOperator() == true)
	{
		reply = ERR_UNKOWNERROR(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "OPER", "You are already logged as a network operator");
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
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
		if (op_pass[place] == args[1]) {
			reply = RPL_YOUREOPER(tmp->getNickname());
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
			reply = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
			tmp->setOperator(true);
		}
		else {
			reply = ERR_PASSWDMISMATCH(tmp->getNickname());
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		}
	}
	else if (client_ip == ip ) {
		reply = add_in_config(args[0], args[1], tmp);
		if (!reply.empty()) {
			send(tmp->getFd(), reply.c_str(), reply.size(), 0);
			return ;
		}
		reply = RPL_YOUREOPER(tmp->getNickname());
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		reply = RPL_oMODE(tmp->getNickname(), tmp->getUsername(), tmp->getHost(), "+o");
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
		tmp->setOperator(true);
	}
	else {
		reply = ERR_NOOPERHOST(tmp->getNickname(), tmp->getUsername(), tmp->getHost());
		send(tmp->getFd(), reply.c_str(), reply.size(), 0);
	}
	confFile.close();
}
