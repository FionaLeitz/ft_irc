#ifndef IRC_H
# define IRC_H

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <errno.h>
# include <arpa/inet.h>
# include <poll.h>
# include <stdio.h>
# include <cstdlib>
# include <map>
# include <vector>
# include <cctype>
# include "Client.hpp"
# include "Channel.hpp"
# include <sstream>

#define USER_ID(nickname, username) (nickname + "!" + username + "@host")
#define RPL_WELCOME(nickname, username) (":server 001 " + nickname + " :Welcome to the test IRC Network " + USER_ID(nickname, username) + "\r\n")
#define RPL_JOIN(nickname, username, channel) (":" + USER_ID(nickname, username) + " JOIN " + channel + "\r\n")
#define RPL_MSG(nickname, username, dest, message)(":" + USER_ID(nickname, username) + " PRIVMSG " + dest + " :" + message + "\r\n");


typedef struct	s_context {
	// a changer, int vers std::string pour le nickname
	std::map<int, Client>	clients;
	std::map<std::string, Channel>	channels;
}			t_context;

typedef struct s_func_ptr
{
	std::string	name;
	void (*ptr)(Client *tmp, struct pollfd *fds, int i, std::string *args);
}	t_func_ptr;

#endif
