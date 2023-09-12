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

typedef struct s_data
{
    std::string prefix;
    std::string command;
    std::string arguments;
    std::string trail;
} Data;

class Server
{
	private:
		const size_t			_port;
		const std::string		_password;
		const std::string		_name;
		
		std::map<int, User *>		_users;
		std::vector<Channel *>	_channels;
		std::vector<pollfd>		_sockets;

		sockaddr_in 			_address;
		socklen_t				_addressSize;
		pollfd					_listener;

		bool					_enabled;

		void    removeSocket(pollfd& socket);

		void	handleUserConnection();
		
		void	handleUserDisconnect(const pollfd& connectionInfo);
		
		std::string readUserInput(pollfd& connectionInfo);
		
		std::vector<std::string> handleUserInput(User *user, std::string &input);
		
		Data parseUserInput(std::string& input);
		
		std::vector<Data> parseUserData(std::vector<std::string>& data);

		bool	handleUserData(pollfd& connectionInfo);

	
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

		void enable();

		void disable();

		//void	startServer();
};
