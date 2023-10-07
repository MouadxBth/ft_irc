#include <cerrno>
#include <cstring>
#include <unistd.h>

#include "Server.hpp"
#include "User.hpp"
#include "CommandManager.hpp"
#include "Utilities.hpp"

void Server::enable()
{
	std::cout << currentTimestamp() << " Starting server..." << std::endl;
	_enabled = true;
	int	 socketsReadyForIO = 0;

	std::vector<pollfd>::iterator currentSocket;

	while (_enabled)
	{
		socketsReadyForIO = poll(_sockets.data(), _sockets.size(), 0);
		
		if (socketsReadyForIO < 0 && _enabled)
		{
			std::cerr << "Error : An error has occurred during the processing of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			return ;
		}
		
		// Idle
		if (!socketsReadyForIO)
			continue;

		if (_sockets[0].revents & POLLIN)
			handleUserConnection();

		for (std::vector<pollfd>::iterator it = _sockets.begin() + 1; it != _sockets.end();)
		{
			currentSocket = findSocket(_socketsToBeRemoved, it->fd);

			if (currentSocket != _socketsToBeRemoved.end())
			{
				it = _sockets.erase(it);
				_socketsToBeRemoved.erase(currentSocket);
				continue ;
			}

			if (_enabled && it->revents & POLLIN)
			{
				handleUserData(it->fd);
				it++;
			}
			else if (_enabled && it->revents & POLLHUP)
			{
				handleUserDisconnection(it->fd);
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

	cleanConnectedUsers();

	cleanAuthenticatedUsers();

	cleanChannels();

	_channels.clear();

	CommandManager *commandManager = CommandManager::getInstance();

    commandManager->cleanUp();

	delete commandManager;
	
	close(_listener.fd);
}