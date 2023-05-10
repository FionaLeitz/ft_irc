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
# include <cctype>

#define USER_ID(nickname, username) (nickname + "!" + username + "@host")
#define RPL_WELCOME(nickname, username) (":server 001 " + nickname + " :Welcome to the test IRC Network " + USER_ID(nickname, username) + "\r\n")
#define RPL_JOIN(nickname, username, channel) (":" + USER_ID(nickname, username) + " JOIN " + channel + "\r\n")


typedef struct	s_client {
	std::string	username;
	std::string	nickname;
	std::string	buffer;
	int			fd;
}			t_client;

#endif
