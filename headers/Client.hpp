#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.h"

class	Client {
	public:
		Client( int fd );
		Client( const Client & value );
		~Client( void );
		
		void			add_buff( char *buffer );
		void			clear( void );

		const std::string &			getUsername( void ) const;
		const std::string &			getNickname( void ) const;
		const std::string &			getBuffer( void ) const;
		const int &					getFd( void ) const;

	private:
		std::string	_username;
		std::string	_nickname;
		std::string	_buffer;
		int			_fd;

		Client( void );

		Client &		operator=( const Client & rhs );
	
};

#endif
