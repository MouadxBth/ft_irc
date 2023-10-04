#pragma once

# include <string>
# include <vector>
# include <map>
# include <set>
# include <poll.h>

# include "Channel.hpp"

class CommandManager;
class Command;
class User;

class Server
{
	private:
		static Server 			*_instance;

		bool					_enabled;

		const size_t			_port;

		const std::string		_password;
		const std::string		_name;
		const std::string		_version;
		const std::string		_creationDate;
		const std::string		_userModes;
		const std::string		_channelModes;

		const std::vector<std::string>		_motd;

		std::string				_hostname;

		std::set<std::string> _reservedNicknames;
		std::set<std::string> _restrictedNicknames;
		
		std::map<int, User *>				_connectedUsers;
		std::map<std::string, User *>		_authenticatedUsers;
		std::map<std::string, Channel *>	_channels;
	
		std::set<std::string>	_channelsToBeRemoved;

		std::vector<pollfd>		_sockets;
		std::vector<pollfd>		_socketsToBeRemoved;

		pollfd					_listener;

	protected:

		Server();
		Server(const Server& instance);
		Server(const size_t port, const std::string password);

		void	handleUserConnection();
		void	handleUserDisconnection(int fd);
		bool	handleUserData(int fd);

	public:
		~Server();
		Server& operator=(const Server& instance);

		static Server* getInstance();
		static Server* createInstance(size_t port, std::string& password);

		bool	isEnabled() const;
		
		size_t						getPort() const;
		const std::string&			getPassword() const;
		const std::string&			getName() const;
		const std::string&			getVersion() const;
		const std::string&			getCreationDate() const;
		const std::string&			getUserModes() const;
		const std::string&			getChannelModes() const;
		
		const std::vector<std::string>&					getMotd() const;
		
		const std::set<std::string>&					getReservedNicknames() const;
		const std::set<std::string>&					getRestrictedNicknames() const;

		const std::map<int, User *>&					getConnectedUsers() const;
		const std::map<std::string, User *>&			getAuthenticatedUsers() const;
		const std::map<std::string, Channel *>&			getChannels() const;

		User						*getUser(int fd);
		User						*getUser(const std::string& name);
		Channel						*getChannel(const std::string& name);

		bool	addReservedNickname(const std::string& nickname);
		bool	removeReservedNickname(const std::string& nickname);

		bool	addRestrictedNickname(const std::string& nickname);
		bool	removeRestrictedNickname(const std::string& nickname);

		bool	authenticateUser(User *user);

		bool	removeUser(User *user);

		void	cleanConnectedUsers();

		void	cleanAuthenticatedUsers();

		void	addChannel(Channel *channel);

		bool	removeChannel(const std::string& name);

		void	cleanChannels();

		void configure();

		void enable();

		void disable();

};
