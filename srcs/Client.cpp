#include "../headers/Client.hpp"

Client::Client( void ) {}

Client::Client( const Client & value ) {
	this->_username = value.getUsername();
	this->_nickname = value.getNickname();
	this->_buffer = value.getBuffer();
	this->_fd = value.getFd();
}

Client &	Client::operator=( const Client & rhs ) {
	this->_username = rhs.getUsername();
	this->_nickname = rhs.getNickname();
	this->_buffer = rhs.getBuffer();
	this->_fd = rhs.getFd();
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

const std::string &	Client::getUsername( void ) const {
	return this->_username;
}

const std::string &	Client::getNickname( void ) const {
	return this->_nickname;
}

const std::string &	Client::getBuffer( void ) const {
	return this->_buffer;
}

const int &	Client::getFd( void ) const {
	return this->_fd;
}

void	Client::setNickname(std::string nick) {
	this->_nickname = nick;
}

void	Client::setUsername(std::string username) {
	this->_username = username;
}
