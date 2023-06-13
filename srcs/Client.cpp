#include "../headers/irc.h"

Client::Client( void ) {}

Client::Client( const Client & value ) {
	this->_canConnect = false;
	this->_username = value.getUsername();
	this->_nickname = value.getNickname();
	this->_buffer = value.getBuffer();
	this->_fd = value.getFd();
	this->_host = value.getHost();
}

Client::Client( const Client & value, struct sockaddr_in &ip) {
	std::stringstream ss;

	this->_canConnect = value.canConnect();
	this->_username = value.getUsername();
	this->_nickname = value.getNickname();
	this->_buffer = value.getBuffer();
	this->_fd = value.getFd();
	this->_ip = ip;
		
	ss << inet_ntoa(this->_ip.sin_addr) << ":" << ntohs(this->_ip.sin_port);
	this->_host = ss.str();
}

Client &	Client::operator=( const Client & rhs ) {
	this->_canConnect = rhs.canConnect();
	this->_username = rhs.getUsername();
	this->_nickname = rhs.getNickname();
	this->_buffer = rhs.getBuffer();
	this->_fd = rhs.getFd();
	this->_host = rhs.getHost();
	return *this;
}

Client::~Client( void ) {
}

Client::Client( int fd ) : _fd( fd ) {

}

void	Client::clear( void ) {
	this->_buffer.clear();
}

void	Client::add_buff( char *buffer ) {
	this->_buffer.insert( this->_buffer.size(), buffer );
}

// void	Client::removefromall( std::map<std::string, Channel> channels ) {
// 	for ( int count = this->channelList.size() - 1; count >= 0; count-- ) {
// 		channels[this->channelList[count].first].
// 	}
// }

bool	Client::canConnect(void) const {
	return this->_canConnect;
}

const std::string &	Client::getUsername( void ) const {
	return this->_username;
}

const std::string &	Client::getNickname( void ) const {
	return this->_nickname;
}

const std::string &	Client::getHost( void ) const {
	return this->_host;
}

const std::string &	Client::getBuffer( void ) const {
	return this->_buffer;
}

const int &	Client::getFd( void ) const {
	return this->_fd;
}


void	Client::setCanConnect(bool auth) {
	this->_canConnect = auth;
}

void	Client::setNickname(std::string nick) {
	this->_nickname = nick;
}

void	Client::setUsername(std::string username) {
	this->_username = username;
}

void	Client::setHost(std::string host) {
	this->_host = host;
}

const struct sockaddr_in	&Client::getIp( void ) const {
	return this->_ip;
}

// const std::vector<std::string>	&Client::getChannelList(void) const
// {
// 	return this->_channelList;
// }

const std::map<std::string, int>	&Client::getChannelList(void) const
{
	return this->_channelList;
}

void	Client::addChannel(std::string name)
{
	_channelList[name] = 1;
}

void	Client::addInviteChannel(std::string name)
{
	_channelList[name] = 0;
}

// void	Client::removeChannel(std::string name)
// {
// 	std::vector<std::string>::iterator it;

// 	 for (it = _channelList.begin(); it != _channelList.end(); ++it)
// 	 {
//         if (*it == name)
// 		{
//             _channelList.erase(it);
// 			break ;
//         }
//     }
// }

void	Client::removeChannel(std::string name)
{
	std::map<std::string, int>::iterator it;

 	it = _channelList.find(name);
  	_channelList.erase (it);
}

void	Client::printChannelList(void) const
{
       // parcours la map et affiche les clés et les valeurs
    for (std::map<std::string, int>::const_iterator it = _channelList.begin(); it != _channelList.end(); ++it) {
        std::cout << it->first << " (" << it->second << ")" << std::endl;
    }
}

void	Client::leaveAllChannels(std::map<std::string, Channel>	channels_map, std::string rpl)
{
	for (std::map<std::string, int>::iterator it2 = _channelList.begin(); it2 != _channelList.end(); )//++it2)
	{
		if (it2->second == 1)
		{
			std::cout << "sending goodbye to channel " << it2->first << std::endl;
			channels_map[it2->first].sendMessage(rpl, _fd);
			// channels_map[it2->first].sendToAll(rpl);
			channels_map[it2->first].suppress_client(_nickname);
			this->_channelList.erase(it2++);
		}
		else
			it2++;
	}
}