#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <algorithm>

typedef void (*FunctionType)(const Client&);

class	Channel {
	public:
		Channel( std::string name, std::string mode, Client new_client );
		Channel( const Channel & value );
		Channel(void);
		~Channel( void );

		Channel &		operator=( const Channel & rhs );

		void				add_client( Client new_client );
		void				add_operator( Client new_client );
		void				suppress_client( std::string nick );
		void				suppress_operator( std::string nick );

		const std::string &						getName( void ) const;
		const std::string &						getMode( void ) const;
		const std::string &						getTopic( void ) const;
		const std::map<std::string, Client> &	getClientlist( void ) const;
		const std::map<std::string, Client> &	getOperators( void ) const;

		void									setMode( std::string mode );
		void									setTopic( std::string topic );

		void									appliquerFonction(FunctionType function);
		void									sendMessage(std::string response, int currentFd);
		void									sendToAll(std::string response);
		void									sendToOne(std::string response, int fd);
		bool									isUserThere(std::string nick);


	private:
		std::string						_name;
		std::string						_mode;
		std::string						_topic;
		std::map<std::string, Client>	_clientlist;
		std::map<std::string, Client>	_operators;


	
};

#endif
