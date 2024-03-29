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
	this->add_operator( new_client.getNickname() );
}

void	Channel::add_client( Client new_client ) {
	this->_clientlist.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::add_operator( std::string new_client ) {
	this->_operators.insert( new_client );
}

void	Channel::suppress_client( std::string nick ) {
	this->_clientlist.erase( nick );
	this->_operators.erase( nick );
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

const std::string &	Channel::getPassword( void ) const {
	return this->_password;
}

const unsigned long &Channel::getSizemax( void ) const {
	return this->_sizemax;
}

const std::map<std::string, Client> &	Channel::getClientlist( void ) const {
	return this->_clientlist;
}

const std::set<std::string> &	Channel::getOperators( void ) const {
	return this->_operators;
}

void	Channel::setMode( std::string mode ) {
	this->_mode = mode;
}

void	Channel::setTopic( std::string topic ) {
	this->_topic = topic;
}

void	Channel::setPassword( std::string password ) {
	this->_password = password;
}

void	Channel::setSizemax( int sizemax ) {
	this->_sizemax = sizemax;
}

// void Channel::appliquerFonction(FunctionType function)
// {
//     std::map<std::string, Client>::iterator it;
//     for (it = _clientlist.begin(); it != _clientlist.end(); ++it)
//     {
//         function(it->second);
//     }
// }

void	Channel::sendMessage(std::string reply, int currentFd)
{
	int	fd;

	std::map<std::string, Client>::const_iterator it;
    for (it = this->_clientlist.begin(); it != this->_clientlist.end(); ++it)
    {
		fd = it->second.getFd();
		if (fd != currentFd)
			send(fd, reply.c_str(), reply.length(), 0);
    }
}

void	Channel::sendToAll(std::string reply)
{
	std::map<std::string, Client>::const_iterator it;

    for (it = this->_clientlist.begin(); it != this->_clientlist.end(); ++it)
    	send(it->second.getFd(), reply.c_str(), reply.length(), 0);
}

bool	Channel::isUserThere(std::string nick)
{
	if (this->_clientlist.find(nick) == this->_clientlist.end())
		return false;
	return true;
}


bool	Channel::isUserOperator(const Client &client) const {

	std::string reply;

	if (this->_operators.find(client.getNickname()) != this->_operators.end()) //si le user est un operateur du channel
		return true ;
	else
	{
		reply = ERR_CHANOPRIVSNEEDED(client.getNickname(), this->_name);
		send(client.getFd(), reply.c_str(), reply.size(), 0);
		return false ;
	}
}