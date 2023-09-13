#include <cerrno>
#include <cstring>

#include "Server.hpp"
#include "CommandManager.hpp"
#include "PassCommand.hpp"
#include "UserCommand.hpp"
#include "NickCommand.hpp"

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

std::vector<std::string>&	Server::getReservedNicknames()
{
	return (this->_reservedNicknames);
}

std::vector<std::string>&	Server::getRestrictedNicknames()
{
	return (this->_restrictedNicknames);
}

const User *Server::getUser(int fd) const
{
	return (this->_users.find(fd)->second);
}

const User *Server::getUser(const std::string& nickname)
{
	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second && it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
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

	_addressSize = sizeof(_address);

	//  Set the events member of poll_fd to monitor for input and output events
    _listener.events  = POLLIN | POLLOUT;
    _listener.revents = 0;

	// Bind the socket to the IP address and port of the server and check the status 
    if (bind(_listener.fd, reinterpret_cast<struct sockaddr *>(&_address), _addressSize) < 0)
        (std::cerr << "Error : Binding Failed due to: " 
			<< strerror(errno) 
			<< std::endl, 
			std::exit(EXIT_FAILURE));

	 // Listening for incoming connection requests and check the status
    if (listen(_listener.fd, SOMAXCONN) < 0)
	{
    	(std::cerr << "Error : Listening Failed !" << std::endl, std::exit(EXIT_FAILURE));
	}

	_sockets.push_back(_listener);

	_commandManager = new CommandManager();

	_commandManager->setServer(this);

	_commandManager->registerCommands(new PassCommand(),
		new NickCommand(),
		new UserCommand(),
		NULL);
}


void Server::enable()
{
	std::cout << currentTimestamp() << " Starting server..." << std::endl;
	_enabled = true;
	int	 eventsToBeProcessed = 0;

	while (_enabled)
	{
		eventsToBeProcessed = poll(_sockets.data(), _sockets.size(), 0);
		
		if (eventsToBeProcessed < 0 && _enabled)
		{
			std::cerr << "Error : An error has occurred during the processing of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			continue;	//std::exit(EXIT_FAILURE); // should probably just return to free the allocated memory elements
		}

		if (!eventsToBeProcessed)
			continue;

		if (_sockets[0].revents & POLLIN)
			handleUserConnection();

		for (std::vector<pollfd>::iterator it = _sockets.begin() + 1; it != _sockets.end();)
		{
			if (_enabled && it->revents & POLLIN)
			{
				//std::cout << "Receiving user data..." << std::endl;
				it = (!handleUserData(*it)) ? _sockets.erase(it) : it + 1;
			}
			else if (_enabled && it->revents & POLLHUP)
			{
				//std::cout << "User disconnected..." << std::endl;
				handleUserDisconnect(*it);
				it = _sockets.erase(it);
			}
			else
				it++;
		}
        _sockets[0].revents = 0;
	}
}

void Server::disable()
{
	std::cout << currentTimestamp() << " Disabling server..." << std::endl;
	_enabled = false;

	std::string message = currentTimestamp() + " Server has been shut down.\n";

	for (std::map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (!it->second)
			continue;

		it->second->sendMessage(message);
		handleUserDisconnect(it->second->getUserSocket());
	}

	_users.clear();
	_sockets.clear();
	
	close(_listener.fd);
}