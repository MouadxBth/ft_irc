#include "../include/Server.hpp"
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <cerrno>

Server::Server()
	:_port(6667),
	_password("password"),
	_name("ircserv.1337.ma")
{}

Server::Server(const Server& instance)
	: _port(instance.getPort()),
	_password(instance.getPassword()),
	_name(instance.getName())
{
	*this = instance;
}

Server& Server::operator=(const Server& instance)
{
	if (this != &instance)
	{
		_users.clear();
		_channels.clear();

		_users = instance._users;
		_channels = instance._channels;

		_address = instance._address;
		_listener = instance._listener;
	}
	return (*this);
}

Server::Server(const size_t port, const std::string password)
	:_port(port),
	_password(password),
	_name("ircserv.1337.ma")
{}

Server::~Server()
{
	for (std::map<int, User *>::const_iterator it = getUsers().begin();
		it != getUsers().end();
		it++)
	{
		close(it->second->getUserSocket().fd);
		delete it->second;
	}

	for (std::vector<Channel *>::const_iterator it = getChannels().begin();
		it != getChannels().end();
		it++)
	{
		delete *it;
	}

	_users.clear();
	_channels.clear();
}

const std::string&    Server::getName() const
{
    return (this->_name);
}

const std::string&    Server::getPassword() const
{
    return (this->_password);
}

size_t		Server::getPort() const
{
    return (this->_port);
}

std::map<int, User *>&		Server::getUsers()
{
    return (this->_users);
}

std::vector<Channel *>&    Server::getChannels()
{
    return (this->_channels);
}

const User *Server::getUser(int fd) const
{
	return (this->_users.find(fd)->second);
}

void	Server::removeUser(int fd)
{
	const User *user = getUser(fd);

	close(fd);
	_users.erase(fd);

	if (user)
	{
		for (std::vector<Channel *>::const_iterator it = getChannels().begin();
			it != getChannels().end();
			it++)
		{
			(*it)->removeUser(user->getNickname());
		}
		delete user;
	}
}

void	Server::configure(void)
{
    // Creation socket for the server and check it's status 
    _listener.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listener.fd < 0)
        (std::cerr << "Error : Creation of Socket Failed !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));
    {
		// Set the socket to non-blocking mode
        int enabled = 1;
        setsockopt (_listener.fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled) );
    }
    if (fcntl(_listener.fd, F_SETFL, O_NONBLOCK) < 0)
		(std::cerr << "Error : Unable to set Socket to non-blocking mode !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));

	_address.sin_family      = AF_INET;
    _address.sin_port        = htons(this->_port);
    _address.sin_addr.s_addr = INADDR_ANY;

	//  Set the events member of poll_fd to monitor for input and output events
    _listener.events  = POLLIN | POLLOUT;
    _listener.revents = 0;

	// Bind the socket to the IP address and port of the server and check the status 
    if (bind(_listener.fd, reinterpret_cast<struct sockaddr *>(&_address), _addressSize) < 0)
        (std::cerr << "Error : Binding Failed !" << std::endl, std::exit(EXIT_FAILURE));

	 // Listening for incoming connection requests and check the status
    if (listen(_listener.fd, SOMAXCONN) < 0)
        (std::cerr << "Error : Listening Failed !" << std::endl, std::exit(EXIT_FAILURE));
}

pollfd Server::handleUserConnection()
{
	int newUserSocket;
	sockaddr_in newUserAddress;
	// accept the new connection and create newUserSocket
	newUserSocket = accept(_listener.fd,
		reinterpret_cast<struct sockaddr *>(&newUserAddress),
		reinterpret_cast<socklen_t *>(&newUserAddress));
	
	if ( newUserSocket < 0)
	{
		std::cerr << "Error : Unable to accept new connection !\n"
			<< strerror(errno)
			<< std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::cout << "A User has joined the server!" << std::endl;
	// set the newUserSocket to the non-blocking mode
	if (fcntl(newUserSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : Accept Failed ! Unable to set user's file descriptor to non blocking mode.\n"
			<< strerror(errno)
			<< std::endl;
		std::exit(EXIT_FAILURE);
	}
	pollfd  newUserFd;
	
	// fill the new pollfd struct of the newUser 
	newUserFd.fd      = newUserSocket;
	newUserFd.events  = POLLIN | POLLOUT;
	newUserFd.revents = 0;

	// create a new user with fd
	User    *newUser = new User();
	
	newUser->setAddress(newUserAddress);
	newUser->setHostname(obtain_hostname(newUserAddress));
	// add it to the Users map
	_users[newUserFd.fd] = newUser;
	return (newUserFd);
}

void Server::handleUserDisconnect(pollfd& connectionInfo)
{
	removeUser(connectionInfo.fd);
	
	print_timestamped();

	std::cout << "User: [" << connectionInfo.fd << "] " 
		<< _users[connectionInfo.fd]->getUsername() 
		<< " has disconnected" 
		<< std::endl;
}

void Server::handleUserData(pollfd& connectionInfo)
{
	int readData = 1;
	char    buffer[4096];
	
	memset(buffer,'\0', 4096);
	
	// set recv on non blocking mode
	readData = recv(connectionInfo.fd, buffer, 4096, MSG_DONTWAIT);

	if (readData < 0)
	{
		std::cerr << "Error : Failed to receive data from a socket descriptor !.\n"
			<< strerror(errno)
			<< std::endl;
		std::exit(EXIT_FAILURE); // or just return
	}
	
	// there is nothing to read from the client and the file descriptor is closed :
	if (!readData)
	{
		removeUser(connectionInfo.fd);
		return ;
	}

	// handle command
}

void Server::enable()
{
	std::vector<pollfd> sockets;

	sockets.push_back(_listener);

	int	 eventsToBeProcessed = 0;

	while (true)
	{
		eventsToBeProcessed = poll(sockets.data(), sockets.size(), 0);
		
		if (eventsToBeProcessed < 0)
		{
			std::cerr << "Error : An error has occurred during the process of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			std::exit(EXIT_FAILURE); // should probably just return to free the allocated memory elements
		}

		if (!eventsToBeProcessed)
			continue;

		if (sockets[0].revents & POLLIN)
			sockets.push_back(handleUserConnection());
		
		for (std::vector<pollfd>::iterator it = sockets.begin() + 1; it != sockets.end(); it++)
		{
			if (it->revents & POLLIN)
				handleUserData(*it);
			else if (it->revents & POLLHUP)
				handleUserDisconnect(*it);
		}

        sockets[0].revents = 0;
	}
}