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

void	ft_join(t_context *context, Client *tmp, struct pollfd *fds, int i, std::string *args)
{
	std::string	response;
	// (void)context;

	std::cout << "Received command JOIN w args " << args[0] << " and " << args[1] << std::endl;
	std::cout << (*tmp).getNickname() << " veut rejoindre le channel" << args[0] << std::endl;
	if (context->channels.find(args[0]) == context->channels.end())
	{
		context->channels[args[0]] = Channel(args[0], "t", (*tmp));	//ajoute le channel a la map
	}
	else
		context->channels[args[0]].add_client((*tmp));
	(*tmp).addChannel(args[0]);
	response = RPL_JOIN((*tmp).getNickname(), (*tmp).getUsername(), args[0]);
	send(fds[i].fd, response.c_str(), response.length(), 0);
	// context->channels[args[0]].appliquerFonction(afficherNomClient);
	
}
