/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:42:14 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 16:53:54 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>

#include "Server.hpp"
#include "User.hpp"
#include "CommandManager.hpp"

static std::vector<pollfd>::iterator findSocket(std::vector<pollfd>& vec, int fd)
{
    for (std::vector<pollfd>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
        if (it->fd == fd)
            return (it);
    }
    return (vec.end());
}

void Server::enable()
{
	std::cout << currentTimestamp() << " Starting server..." << std::endl;
	_enabled = true;
	int	 eventsToBeProcessed = 0;

	std::vector<pollfd>::iterator currentSocket;

	while (_enabled)
	{
		eventsToBeProcessed = poll(_sockets.data(), _sockets.size(), 0);
		
		if (eventsToBeProcessed < 0 && _enabled)
		{
			std::cerr << "Error : An error has occurred during the processing of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			continue;
		}

		if (!eventsToBeProcessed)
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
				//std::cout << "Receiving user data..." << std::endl;
				it = (!handleUserData(*it)) ? _sockets.erase(it) : it + 1;
			}
			else if (_enabled && it->revents & POLLHUP)
			{
				//std::cout << "User disconnected..." << std::endl;
				handleUserDisconnection(*it);
				//it = _sockets.erase(it);
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
	std::string message = currentTimestamp() + " Server has been shut down.\n";
	
    _enabled = false;

	for (std::map<int, User *>::const_iterator it = _connectedUsers.begin();
        it != _connectedUsers.end();
        it++)
	{
		if (!it->second)
			continue;

		it->second->sendMessage(message);
		handleUserDisconnection(it->second->getUserSocket());
	}

    for (std::map<std::string, User *>::const_iterator it = _authenticatedUsers.begin();
        it != _authenticatedUsers.end();
        it++)
	{
		if (!it->second)
			continue;

		it->second->sendMessage(message);
		handleUserDisconnection(it->second->getUserSocket());
	}

	cleanChannels();

	CommandManager *commandManager = CommandManager::getInstance();

    commandManager->cleanUp();

	delete commandManager;

    _connectedUsers.clear();
	_authenticatedUsers.clear();
	_sockets.clear();
	
	close(_listener.fd);
}