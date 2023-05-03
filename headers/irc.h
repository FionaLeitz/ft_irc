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

typedef struct	s_client {
	std::string	username;
	std::string	nickname;
	std::string	buffer;

}			t_client;

#endif
