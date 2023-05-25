#ifndef IRC_H
# define IRC_H

# include <iostream>
# include <algorithm>
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
#define	RPL_KICK(nickname, username, channel, target, reason)(":" + USER_ID(nickname, username) + " KICK " + channel + " " + target + " :" + reason + "\r\n")
#define RPL_MSG(nickname, username, dest, message) (":" + USER_ID(nickname, username) + " PRIVMSG " + dest + " :" + message + "\r\n")
#define RPL_QUIT(nickname, username, message) (":server 301 " + USER_ID(nickname, username) + " " + nickname + " Quit: " + message + "\r\n")
#define RPL_TOPIC(nickname, channel, topic) (":server 332 " + nickname + " " + channel + " :" + topic + "\r\n")
#define RPL_NOTOPIC(nickname, channel) (":server 331 " + nickname + " " + channel + " :No topic is set\r\n")
#define RPL_CHANGETOPIC(nickname, username, channel, topic) (":" + USER_ID(nickname, username) + " TOPIC " + channel + " " + topic + "\r\n")
#define RPL_PING(nickname, username, token) (":" + USER_ID(nickname, username) + " PONG server_name " + token + "\r\n")
#define ERR_NOSUCHNICK(nickname, username, target) (":server 401 " + USER_ID(nickname, username) + " " + target + " :No such nick\r\n")
#define ERR_NOSUCHCHANNEL(nickname, username, target) (":server 403 " + USER_ID(nickname, username) + " " + target + " :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(nickname, username, channel, target) (":server 441 " + USER_ID(nickname, username) + " " + channel + " " + target + " :They aren't on that channel\r\n")
// #define ERR_USERNOTINCHANNEL(nickname, username, channel, target) (":server 441 " + USER_ID(nickname, username) + " " + target + " :They aren't on that channel\r\n")
// 	normalement comme ca, et le msg d'erreur arrive sur l'onglet principal (et non pas sur le channel)
// ca semble etre ca sur les vrais serveurs
#define ERR_NOTEXTTOSEND(nickname, username) (":server 412 " + USER_ID(nickname, username) + " :No text to send\r\n")
#define ERR_NEEDMOREPARAMS(nickname, username, command) (":server 461 " + USER_ID(nickname, username) + " " + command + " :Not enough parameters\r\n")


typedef struct	s_context {
	// a changer, int vers std::string pour le nickname
	std::map<int, Client>			clients;
	std::map<std::string, Channel>	channels;
	int								password;
	int								socket_nbr[1];
}			t_context;

typedef struct s_func_ptr
{
	std::string	name;
	void (*ptr)(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
}	t_func_ptr;

void	ft_quit(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_user(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_nick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_privmsg(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_who(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_mode(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_join(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_invite(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_kill(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_list(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_part(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_ping(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_restart(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);
void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args);


void	sendToAllClients(const std::map<std::string, Client> &clientList, std::string response);

#endif
