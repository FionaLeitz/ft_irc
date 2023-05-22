#include "../headers/irc.h"

Channel::Channel( void ) {}

Channel::Channel( const Channel & value ) {
	this->_name = value.getName();
	this->_mode = value.getMode();
	this->_topic = value.getTopic();
	this->_clientlist = value.getClientlist();
}

Channel &	Channel::operator=( const Channel & rhs ) {
	this->_name = rhs.getName();
	this->_mode = rhs.getMode();
	this->_topic = rhs.getTopic();
	this->_clientlist = rhs.getClientlist();
	return *this;
}

Channel::~Channel( void ) {
}

Channel::Channel( std::string name, std::string mode, Client new_client ) :
	_name( name ), _mode( mode ) {
	this->add_client( new_client );
	this->add_operator( new_client );
	// this->_clientlist.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::add_client( Client new_client ) {
	this->_clientlist.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::add_operator( Client new_client ) {
	this->_operators.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::suppress_client( std::string nick ) {
	this->_clientlist.erase( nick );
}

void	Channel::suppress_operator( std::string nick ) {
	this->_operators.erase( nick );
}

const std::string &	Channel::getName( void ) const {
	return this->_name;
}

const std::string &	Channel::getMode( void ) const {
	return this->_mode;
}

const std::string &	Channel::getTopic( void ) const {
	return this->_topic;
}

const std::map<std::string, Client> &	Channel::getClientlist( void ) const {
	return this->_clientlist;
}

const std::map<std::string, Client> &	Channel::getOperators( void ) const {
	return this->_operators;
}

void	Channel::setMode( std::string mode ) {
	this->_mode = mode;
}

void	Channel::setTopic( std::string topic ) {
	this->_topic = topic;
}

void Channel::appliquerFonction(FunctionType function)
{
    std::map<std::string, Client>::iterator it;
    for (it = _clientlist.begin(); it != _clientlist.end(); ++it)
    {
        function(it->second);
    }
}

void	Channel::sendMessage(std::string response, int currentFd)
{
	int	fd;

	std::map<std::string, Client>::const_iterator it;
    for (it = this->_clientlist.begin(); it != this->_clientlist.end(); ++it)
    {
		
		fd = it->second.getFd();
		if (fd != currentFd)
		{
			std::cout << "sending message to " << fd << std::endl;
			send(fd, response.c_str(), response.length(), 0);
		}
    }
}
