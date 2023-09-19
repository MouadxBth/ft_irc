#pragma once

# include <string>
# include <sstream>
# include <vector>
# include <map>

#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <vector>
#include <poll.h>
#include <iostream>
#include <arpa/inet.h>

# include "CommandData.hpp"
# include "Channel.hpp"
# include "Utilities.hpp"

class CommandManager;
class Command;
class User;

class Server
{
	private:
		static Server 					*_instance;

		const size_t			_port;
		const std::string		_password;
		const std::string		_name;
		const std::string		_version;
		const std::string		_creationDate;
		const std::string		_userModes;
		const std::string		_channelModes;
		
		const std::vector<std::string>		_motd;
		
		std::map<int, User *>				_connectedUsers;
		std::map<std::string, User *>		_authenticatedUsers;
		std::map<std::string, Channel *>	_channels;
		
		std::vector<pollfd>		_sockets;
		std::vector<pollfd>		_socketsToBeRemoved;

		std::vector<std::string>	_channelsToBeRemoved;

		std::vector<std::string> _reservedNicknames;
		std::vector<std::string> _restrictedNicknames;

		sockaddr_in 			_address;
		socklen_t				_addressSize;
		pollfd					_listener;

		bool					_enabled;

	protected:

		void	handleUserConnection();
		void	handleUserDisconnection(const pollfd& connectionInfo);
		bool	handleUserData(pollfd& connectionInfo);

	public:
		Server();
		~Server();
		Server(const Server& instance);
		Server& operator=(const Server& instance);
		
		Server(const size_t port, const std::string password);
		
		size_t						getPort() const;
		const std::string&			getPassword() const;
		const std::string&			getName() const;
		const std::string&			getVersion() const;
		const std::string&			getCreationDate() const;
		const std::string&			getUserModes() const;
		const std::string&			getChannelModes() const;
		
		const std::vector<std::string>&			getMotd() const;


		std::map<int, User *>&					getConnectedUsers();
		std::map<std::string, User *>&			getAuthenticatedUsers();
		std::map<std::string, Channel *>&		getChannels();

		std::vector<std::string>&	getReservedNicknames();
		std::vector<std::string>&	getRestrictedNicknames();

		User						*getConnectedUser(int fd);
		
		User						*getAuthenticatedUser(const std::string& nickname);
		User						*getAuthenticatedUser(int fd);
		
		User						*getUser(int fd);
		
		Channel						*getChannel(const std::string& name);

		void	removeUser(User *user);

		void	removeChannel(const std::string& name);

		void	cleanChannels();

		void configure();

		void enable();

		void disable();

		static Server* getInstance();

		static Server* createInstance(size_t port, std::string& password);

};
