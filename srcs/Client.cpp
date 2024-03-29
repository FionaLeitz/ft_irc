#include "../headers/irc.h"

Client::Client( void ) {}

Client::Client( const Client & value ) {
	this->_canConnect = 0;
	this->_pass = false;
	this->_username = value.getUsername();
	this->_nickname = value.getNickname();
	this->_buffer = value.getBuffer();
	this->_fd = value.getFd();
	this->_host = value.getHost();
	this->_serverOperator = false;
}

Client::Client( const Client & value, struct sockaddr_in &ip) {
	std::stringstream ss;

	this->_canConnect = value.canConnect();
	this->_pass = value.getPassBool();
	this->_username = value.getUsername();
	this->_nickname = value.getNickname();
	this->_buffer = value.getBuffer();
	this->_fd = value.getFd();
	this->_ip = ip;
	this->_serverOperator = value.getOperator();
		
	ss << inet_ntoa(this->_ip.sin_addr) << ":" << ntohs(this->_ip.sin_port);
	this->_host = ss.str();
}

Client &	Client::operator=( const Client & rhs ) {
	this->_canConnect = rhs.canConnect();
	this->_pass = rhs.getPassBool();
	this->_username = rhs.getUsername();
	this->_nickname = rhs.getNickname();
	this->_buffer = rhs.getBuffer();
	this->_fd = rhs.getFd();
	this->_host = rhs.getHost();
	this->_serverOperator = rhs.getOperator();
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

const int &	Client::canConnect(void) const {
	return this->_canConnect;
}

const bool &	Client::getPassBool(void) const {
	return this->_pass;
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


void	Client::setCanConnect(int auth) {
	this->_canConnect = auth;
}

void	Client::setPassBool(bool auth) {
	this->_pass = auth;
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

const std::map<std::string, int>	&Client::getChannelList(void) const
{
	return this->_channelList;
}

void	Client::setOperator(bool status)
{
	this->_serverOperator = status;
}

const bool &	Client::getOperator( void ) const
{
	return this->_serverOperator;
}


void	Client::addChannel(std::string name)
{
	_channelList[name] = 1;
}

void	Client::addInviteChannel(std::string name)
{
	_channelList[name] = 0;
}

void	Client::removeChannel(t_context *context, std::string name)
{
	std::map<std::string, int>::iterator it;

 	it = _channelList.find(name);
	if (it ==_channelList.end())
		return ;
	suppress_empty_chan( context, it->first );
  	_channelList.erase(it);

}

void	Client::leaveAllChannels(t_context *context, std::string reply)
{
	for (std::map<std::string, int>::iterator it2 = _channelList.begin(); it2 != _channelList.end(); )
	{
		std::map<std::string, int>::iterator	save = it2;
		save++;
		if (it2->second == 1)
		{
			context->channels[it2->first].sendMessage(reply, _fd);
			context->channels[it2->first].suppress_client(_nickname);
			suppress_empty_chan( context, it2->first );
			this->_channelList.erase(it2);
		}
		it2 = save;
	}
}