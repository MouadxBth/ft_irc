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
# include <string.h>
#include <iostream>
#include <arpa/inet.h>
# include "Channel.hpp"
# include "User.hpp"
# include "utils.hpp"

class Server
{
	private:
		const size_t			_port;
		const std::string		_password;
		const std::string		_name;
		
		std::map<int, User *>		_users;
		std::vector<Channel *>	_channels;

		sockaddr_in 			_address;
		socklen_t				_addressSize;
		pollfd					_listener;

		pollfd handleUserConnection();
		void handleUserDisconnect(pollfd& connectionInfo);
		void handleUserData(pollfd& connectionInfo);
	
	public:
		Server();
		~Server();
		Server(const Server& instance);
		Server& operator=(const Server& instance);
		
		Server(const size_t port, const std::string password);
		
		const std::string&			getName() const;
		const std::string&			getPassword() const;
		size_t				getPort() const;

		std::map<int, User *>&		getUsers();
		std::vector<Channel *>&		getChannels();

		const User *getUser(int fd) const;

		void configure();

		void removeUser(int fd);

		void enable();

		//void	startServer();
};
