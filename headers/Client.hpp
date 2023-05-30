#ifndef CLIENT_HPP
# define CLIENT_HPP


class	Client {
	public:
		Client( int fd );
		Client( const Client & value );
		Client( const Client & value, struct sockaddr_in &ip);
		Client( void );
		~Client( void );

		Client &		operator=( const Client & rhs );
		
		void			add_buff( char *buffer );
		void			clear( void );

		const std::string &			getUsername( void ) const;
		const std::string &			getNickname( void ) const;
		const std::string &			getBuffer( void ) const;
		const int &					getFd( void ) const;

		void						setNickname(std::string nick);
		void						setUsername(std::string username);
		const struct sockaddr_in	&getIp(void) const;
		const std::vector<std::string>	&getChannelList(void) const;
		void						addChannel(std::string name);
		void						removeChannel(std::string name);

	private:
		std::string					_username;
		std::string					_nickname;
		std::string					_buffer;
		int							_fd;
		struct sockaddr_in			_ip;
		std::vector<std::string>	_channelList;
		// std::map<std::string>	_channelList;
		// std::vector<std::string>	_inviteChannelList;
	
};

#endif
