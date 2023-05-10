#include "headers/Channel.hpp"

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
	// this->_clientlist.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::add_client( Client new_client ) {
	this->_clientlist.insert( std::map<std::string, Client>::value_type( new_client.getNickname(), new_client ) );
}

void	Channel::suppress_client( std::string nick ) {
	this->_clientlist.erase( nick );
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

void	Channel::setMode( std::string mode ) {
	this->_mode = mode;
}

void	Channel::setTopic( std::string topic ) {
	this->_topic = topic;
}
