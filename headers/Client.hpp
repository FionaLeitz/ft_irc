#ifndef CLIENT_HPP
# define CLIENT_HPP

class	Channel;

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
		// void			removefromall( void );

		const bool &				canConnect(void) const;
		const bool &				getPassBool(void) const;
		const std::string &			getUsername( void ) const;
		const std::string &			getNickname( void ) const;
		const std::string &			getHost( void ) const;
		const std::string &			getBuffer( void ) const;
		const int &					getFd( void ) const;
		const struct sockaddr_in	&getIp(void) const;
		const std::map<std::string, int>	&getChannelList(void) const;
		const bool &				getOperator( void ) const;

		void						setCanConnect(bool auth);
		void						setPassBool(bool auth);
		void						setNickname(std::string nick);
		void						setUsername(std::string username);
		void						setHost(std::string host);
		void						setOperator(bool status);

		void						addChannel(std::string name);
		void						addInviteChannel(std::string name);
		void						removeChannel(std::string name);
		void						leaveAllChannels(std::map<std::string, Channel>	channels_map, std::string rpl);

		void						printChannelList(void) const;


	private:
		bool						_canConnect;
		bool						_pass;
		std::string					_username;
		std::string					_nickname;
		std::string					_host;
		std::string					_buffer;
		int							_fd;
		struct sockaddr_in			_ip;
		std::map<std::string, int>	_channelList;		//nom du channel, int pour dire si le user est membre ou est invite a rejoindre (1 = membre, 0 = invite mais pas encore membre)
		bool						_serverOperator;
		
};

#endif
