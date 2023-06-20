#include "../headers/irc.h"

void afficherNomClient(const Client& client)
{
    std::cout << client.getNickname() << std::endl;
}
void sendToAllClients(const std::map<std::string, Client> &clientList, std::string response)
{
	int	fd;

	std::map<std::string, Client>::const_iterator it;
    for (it = clientList.begin(); it != clientList.end(); ++it)
    {
		fd = it->second.getFd();
		send(fd, response.c_str(), response.length(), 0);
    }

}

void	ft_join(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	response;
	std::string message;
	(void)fds;
	(void)i;
	
	std::cout << "Received command JOIN" << std::endl;
	std::vector<std::string>	channels = ft_split( args[0], "," );
	std::vector<std::string>	passwords;
	if ( args.size() != 1 )
		passwords = ft_split( args[1], "," );
	else {
		for (int count = channels.size(); count > 0; count--)
			passwords.push_back("");
	}
	int	size = channels.size();
	for ( int count = 0; count < size; count++ ) {
		int	save = 0;
		std::vector<std::string>    args2;
        args2.push_back(channels[count]);
		if ( channels[count][0] != '#' ) {
			std::string newchan = "#";
			newchan += channels[count];
			channels[count] = newchan;
			args2.pop_back();
            args2.push_back(channels[count]);
		}
		if (context->channels.find(channels[count]) == context->channels.end())
		{
			context->channels[channels[count]] = Channel(channels[count], "t", (*tmp));	//ajoute le channel a la map
			context->channels[channels[count]].add_operator(tmp->getNickname());
			std::cout << "C'EST UN NOUVEAU CHANNEL !!!" << std::endl;
			save = 2;
		}
		else if (context->channels[channels[count]].isUserThere(tmp->getNickname()) == false) {
			if ( context->channels[channels[count]].getMode().find("k") != std::string::npos ) {
				if ( passwords[count] != context->channels[channels[count]].getPassword() ) {
					response = ERR_BADCHANNELKEY(tmp->getNickname(), channels[count]);
					send(tmp->getFd(), response.c_str(), response.size(), 0);
					save = 1;
				}
			}
			if ( context->channels[channels[count]].getMode().find("i") != std::string::npos && save == 0 && tmp->getChannelList().find(channels[count]) == tmp->getChannelList().end() ) {
				response = ERR_INVITEONLYCHAN(tmp->getNickname(), channels[count]);
				send(tmp->getFd(), response.c_str(), response.size(), 0);
				save = 1;
			}
			if ( context->channels[channels[count]].getMode().find("l") != std::string::npos && save == 0 && context->channels[channels[count]].getClientlist().size() >= context->channels[channels[count]].getSizemax() ) {
				response = ERR_CHANNELISFULL(tmp->getNickname(), channels[count]);
				send(tmp->getFd(), response.c_str(), response.size(), 0);
				save = 1;
			}
		}
		if ((save == 0 && context->channels[channels[count]].isUserThere(tmp->getNickname()) == false) || save == 2) {
			context->channels[channels[count]].add_client((*tmp));
			(*tmp).addChannel(channels[count]);
			response = RPL_JOIN((*tmp).getNickname(), (*tmp).getUsername(), tmp->getHost(), channels[count]);
			context->channels[channels[count]].sendToAll(response);
			ft_names(context, tmp, fds, i, args2);
			check_topic(tmp, context->channels[channels[count]]);
		}
	}

}
