#include "../headers/irc.h"

void afficherNomClient(const Client& client)
{
    std::cout << client.getNickname() << std::endl;
}

// void showAllClients(const std::map<std::string, Client> &clientList)
// {
// 	for_each(clientList.begin(), clientList.end(), Client::getNickname);
// }

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


/*
<channel>{,<channel>} [<key>{,<key>}]
- the JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it.
- the server receiving the command checks whether or not the client can join the given channel, and processes the request.
- servers MUST process the parameters of this command as lists on incoming commands from clients, with the first <key> being
used for the first <channel>, the second <key> being used for the second <channel>, etc.
- while a client is joined to a channel, they receive all relevant information about that channel including the JOIN, PART,
KICK, and MODE messages affecting the channel. They receive all PRIVMSG and NOTICE messages sent to the channel, and they also
receive QUIT messages from other clients joined to the same channel (to let them know those users have left the channel and the network).
This allows them to keep track of other channel members and channel modes.

- If a client’s JOIN command to the server is successful, the server MUST send, in this order:

    1. a JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
    2. the channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), and no message if the channel does not have a topic.
    3. a list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366).
	These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.

The key, client limit , ban - exception, invite-only - exception, and other (depending on server software) channel modes affect whether or
not a given client may join a channel. More information on each of these modes and how they affect the JOIN command is available in their
respective sections.

Servers MAY restrict the number of channels a client may be joined to at one time. This limit SHOULD be defined in the CHANLIMIT RPL_ISUPPORT
parameter. If the client cannot join this channel because they would be over their limit, they will receive an ERR_TOOMANYCHANNELS (405)
reply and the command will fail.

Note that this command also accepts the special argument of ("0", 0x30) instead of any of the usual parameters, which requests that the
sending client leave all channels they are currently connected to. The server will process this command as though the client had sent a
PART command for each channel they are a member of.

This message may be sent from a server to a client to notify the client that someone has joined a channel. In this case, the message
<source> will be the client who is joining, and <channel> will be the channel which that client has joined. Servers SHOULD NOT send
multiple channels in this message to clients, and SHOULD distribute these multiple-channel JOIN messages as a series of messages with a
single channel name on each.
*/

void	ft_join(t_context *context, Client *tmp, struct pollfd *fds, int i, std::vector<std::string> args)
{
	std::string	response;
	(void)fds;
	(void)i;

	std::cout << "Received command JOIN " << std::endl;//w args " << args[0] << " and " << args[1] << std::endl;
	// std::cout << (*tmp).getNickname() << " veut rejoindre le channel" << args[0] << std::endl;

	// args[0] = channels
	// args[1] = mdp
	std::vector<std::string>	channels = ft_split( args[0], "," );
	std::vector<std::string>	passwords;
	if ( args.size() != 1 )
		passwords = ft_split( args[1], "," );
	else {
		for (int count = channels.size(); count > 0; count--)
			passwords.push_back("");
	}

	std::vector<std::string>::iterator	it = channels.begin();
	// std::vector<std::string>::iterator	ite = channels.end();
	std::vector<std::string>::iterator	it_mdp = passwords.begin();
	std::vector<std::string>::iterator	ite_mdp = passwords.end();
	for ( ; it_mdp != ite_mdp; it_mdp++ )
		std::cout << (*it++) << " et " << (*it_mdp) << std::endl;

	int	size = channels.size();
	for ( int count = 0; count < size; count++ ) {
		int	save = 0;
		if (context->channels.find(channels[count]) == context->channels.end())
		{
			context->channels[channels[count]] = Channel(channels[count], "t", (*tmp));	//ajoute le channel a la map
			context->channels[channels[count]].add_operator(tmp->getNickname());
		}
		else if ( context->channels[channels[count]].getMode().find("k") != std::string::npos ) {
			// std::cout << "ON VEUT UN PASSWORD !" << std::endl;
			if ( passwords[count] != context->channels[channels[count]].getPassword() ) {
				response = ERR_BADCHANNELKEY(tmp->getNickname(), channels[count]);
				std::cout << response << std::endl;
				send(tmp->getFd(), response.c_str(), response.size(), 0);
				// message d'erreur : Cannot join #e (Requires keyword
				save = 1;
			}
			// std::cout << "I have the password" << std::endl;
		}
		if ( save == 0 ) {
			context->channels[channels[count]].add_client((*tmp));
			(*tmp).addChannel(channels[count]);
			response = RPL_JOIN((*tmp).getNickname(), (*tmp).getUsername(), channels[count]);
			context->channels[channels[count]].sendToAll(response);
		}
	}
	// send(fds[i].fd, response.c_str(), response.length(), 0);
	// context->channels[args[0]].appliquerFonction(afficherNomClient);	
}


// - join plusieurs channels en meme temps, avec les mdp associes si besoin
// - checker si le user peut rejoindre le channel (limite de channels, limite de membres, mdp, invite only, banni...)
// - une fois le channel rejoint, envoyer a la personne qui join le topic (RPL_TOPIC (332)) et le RPL_TOPICWHOTIME (333)) (ou rien si pas de topic)
// - (a faire ou pas ? ) Servers MAY restrict the number of channels a client may be joined to at one time. This limit SHOULD be defined in the CHANLIMIT RPL_ISUPPORT
// parameter. If the client cannot join this channel because they would be over their limit, they will receive an ERR_TOOMANYCHANNELS (405)
// reply and the command will fail.
// - (a faire ou pas ?) argument "0" (/join 0 ?) qui quitte tous les channels dans lequel est le user
