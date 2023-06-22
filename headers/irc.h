#ifndef IRC_H
# define IRC_H

# include <iostream>
# include <ostream>
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
# include <sstream>
# include <list>
# include <set>
# include <fstream>
# include <signal.h>
# include "Client.hpp"
# include "Channel.hpp"

#define USER_ID(nickname, username, host) (nickname + "!" + username + "@" + host)

#define RPL_WELCOME(nickname, username, host) (":server 001 " + nickname + " :Welcome to the test IRC Network " + USER_ID(nickname, username, host) + "\r\n")
#define RPL_QUIT(nickname, username, host, message) (":server 301 " + USER_ID(nickname, username, host) + " " + nickname + " Quit: " + message + "\r\n")
#define RPL_ENDOFWHO(nickname, channel) (":server 315 " + nickname + " " + channel + " :End of /WHO list.\r\n")
#define RPL_NOTOPIC(nickname, channel) (":server 331 " + nickname + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(nickname, channel, topic) (":server 332 " + nickname + " " + channel + " :" + topic + "\r\n")
#define RPL_CHANNELMODEIS(nickname, channel, sign, mode) (":server 324 " + nickname + " " + channel + sign + mode + "\r\n")
#define RPL_INVITING(nickname, channel, target) (":server 341 " + nickname + " " + target + " :join " + channel + "\r\n")
#define RPL_WHOREPLY(nickname, channel, username, host, usernick) (":server 352 " + nickname + " " + channel + " " + username + " " + host + " server " + usernick + " H :0 " + "realname" + "\r\n")
#define RPL_NAMREPLY(nickname, symbol, channel, prefix, user) (":server 353 " + nickname + " " + symbol + " " + channel + " :" + prefix + user + "\r\n")
#define	RPL_YOUREOPER(nickname) (":server 381 " + nickname + " :You are now an IRC operator\r\n");
#define	RPL_NICK(oldNick, username, host, newNick) (":" + USER_ID(oldNick, username, host) + " NICK :" + newNick + "\r\n");
#define	RPL_FIRSTNICK(nickname) (":server NICK :" + nickname + "\r\n");
#define RPL_JOIN(nickname, username, host, channel) (":" + USER_ID(nickname, username, host) + " JOIN " + channel + "\r\n")
#define	RPL_KICK(nickname, username, host, channel, target, reason)(":" + USER_ID(nickname, username, host) + " KICK " + channel + " " + target + " :" + reason + "\r\n")
#define RPL_MSG(nickname, username, host, dest, message) (":" + USER_ID(nickname, username, host) + " PRIVMSG " + dest + " :" + message + "\r\n")
#define RPL_CHANGETOPIC(nickname, username, host, channel, topic) (":" + USER_ID(nickname, username, host) + " TOPIC " + channel + " " + topic + "\r\n")
#define RPL_PING(nickname, username, host, token) (":" + USER_ID(nickname, username, host) + " PONG server_name " + token + "\r\n")
#define RPL_INVITE_MSG(nickname, username, host, target, channel) (":" + USER_ID(nickname, username, host) + " INVITE " + target + " :" + channel + "\r\n")
#define RPL_MODE(nickname, username, host, channel, mode) (":" + USER_ID(nickname, username, host) + " MODE " + channel + " " + mode + "\r\n")
#define	RPL_oMODE(nickname, username, host, mode) (":" + USER_ID(nickname, username, host) + " MODE " + nickname + " " + mode + "\r\n");
#define RPL_KILL(nickname, username, host, target, message) (":" + USER_ID(nickname, username, host) + " KILL " + target + " " + message + "\r\n")
#define RPL_LISTSTART(nickname, username, host) (":server 321 " + USER_ID(nickname, username, host) + " Channel :Users Name\r\n")
#define RPL_LIST(nickname, username, host, channel, count, topic) (":server 322 " + USER_ID(nickname, username, host) + " " + channel + " " + count + " :" + topic + "\r\n")
#define RPL_LISTEND(nickname, username, host) (":server 323 " + USER_ID(nickname, username, host) + " :End of /LIST\r\n")
#define RPL_PART(nickname, username, host, channel, message) (":" + USER_ID(nickname, username, host) + " PART " + channel + " " + message + "\r\n")
#define RPL_CLOSINGLINK(killer, reason) ("ERROR :Closing Link: server (Killed (" + killer + " (" + reason + ")))\r\n")


#define	ERR_UNKOWNERROR(nickname, username, host, command, message) (":server 400 " + USER_ID(nickname, username, host) + " " + command + " :" + message + "\r\n")
#define ERR_NOSUCHNICK(nickname, username, host, channel, target) (":server 401 " + USER_ID(nickname, username, host) + " " + channel + " " + target + " :No such nick\r\n")
#define ERR_NOSUCHCHANNEL(nickname, username, host, target) (":server 403 " + USER_ID(nickname, username, host) + " " + target + " :No such channel\r\n")
#define ERR_NOTEXTTOSEND(nickname, username, host) (":server 412 " + USER_ID(nickname, username, host) + " :No text to send\r\n")
#define ERR_NONICKNAMEGIVEN() (":server 431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nickname) (":server 432 " + nickname + " " + nickname + "\r\n")
#define ERR_NICKNAMEINUSE(nickname) (":server 433 " + nickname + " " + nickname + "\r\n")
#define ERR_USERNOTINCHANNEL(nickname, username, host, channel, target) (":server 441 " + USER_ID(nickname, username, host) + " " + channel + " " + target + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(nickname, username, host, target) (":server 442 " + USER_ID(nickname, username, host) + " " + target + " :You're not on that channel\r\n")
#define ERR_NOTREGISTERED(nickname)(":server 451 " + nickname + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nickname, channel, command) (":server 461 " + nickname + " " + channel + " " + command + " :Not enough parameters\r\n")
#define ERR_NEEDMOREPARAMS_MODE(nickname, channel, command, mode, syntax) (":server 461 " + nickname + " " + channel + " " + command + " :You must specify a parameter for the " + mode + " mode. Syntax: <" + syntax + ">.\r\n")
#define ERR_ALREADYREGISTERED(nickname, username, host)(":server 462 " + USER_ID(nickname, username, host) + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(nickname) (":server 464 " + nickname + " :Password incorrect\r\n");
#define ERR_BADCHANNELKEY(nickname, channel) (":server 475 " + nickname + " " + channel + " :Cannot join " + channel + " (Requires keyword)\r\n")
#define ERR_CHANOPRIVSNEEDED(nickname, channel) (":server 482 " + nickname + " " + channel + " :You're not channel operator\r\n")
#define ERR_NOOPERHOST(nickname, username, host) (":server 491 " + USER_ID(nickname, username, host) + " :No O-lines for your host\r\n")
#define ERR_INVALIDMODEPARAM(nickname, username, channel, mode) (":server 696 " + nickname + " " + channel + " :Invalid " + mode + " mode parameter(s).\r\n")
#define ERR_INVITEONLYCHAN(nickname, channel) (":server 473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_CHANNELISFULL(nickname, channel) (":server 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_NOPRIVILEGES(nickname, username, host) (":server 481 " + USER_ID(nickname, username, host) + " :Permission Denied- You're not an IRC operator\r\n") 


typedef struct	s_context {
	std::map<int, Client>			clients;
	std::map<std::string, Channel>	channels;
	std::string						password;
	int								socket_nbr[1];
	std::string						op_name;
	std::string						op_password;
	std::string						op_host;
}			t_context;

typedef struct s_func_ptr
{
	std::string	name;
	void (*ptr)(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
}	t_func_ptr;


int		parse_number( const char *number );
void	ft_quit(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_user(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_nick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_privmsg(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_who(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_mode(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_join(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_pass(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_invite(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_kick(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_kill(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_list(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_names(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_oper(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_part(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_ping(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);
void	ft_topic(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args);

int							findClientFd(const std::map<int, Client> &clientList, std::string nick);
void						sendToAllClients(const std::map<std::string, Client> &clientList, std::string response);
std::vector<std::string>	ft_split(std::string to_split, std::string separate);
void						check_topic(Client *tmp, Channel &channel);
int							isAuthorized(const Client &user, std::string command);
bool						isStringAlnum(std::string s);
void						suppress_empty_chan(t_context *context, std::string channel_name);
void						ft_welcome(Client *tmp, struct pollfd *fds, int i);


#endif

