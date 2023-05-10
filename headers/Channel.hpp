#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.h"
# include "Client.hpp"

class	Channel {
	public:
		Channel( std::string name, std::string mode, Client new_client );
		Channel( const Channel & value );
		~Channel( void );

		void				add_client( Client new_client );
		void				suppress_client( std::string nick );

		const std::string &						getName( void ) const;
		const std::string &						getMode( void ) const;
		const std::string &						getTopic( void ) const;
		const std::map<std::string, Client> &	getClientlist( void ) const;
		void									setMode( std::string mode );
		void									setTopic( std::string topic );

	private:
		std::string						_name;
		std::string						_mode;
		std::string						_topic;
		std::map<std::string, Client>	_clientlist;

		Channel( void );

		Channel &		operator=( const Channel & rhs );
		
};

#endif
