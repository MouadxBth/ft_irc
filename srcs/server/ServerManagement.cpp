/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagement.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:42:14 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/21 22:56:15 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>

#include "Server.hpp"
#include "User.hpp"
#include "CommandManager.hpp"

void Server::enable()
{
	std::cout << currentTimestamp() << " Starting server..." << std::endl;
	_enabled = true;

	const int MAX_EVENTS = 4096;

	struct epoll_event events[MAX_EVENTS];

	int	 socketsWithData = 0;
	
	while (_enabled)
	{
		socketsWithData = epoll_wait(_epoll_fd, events, MAX_EVENTS, 15);
		
		if (socketsWithData < 0 && _enabled)
		{
			std::cerr << "Error : An error has occurred during the processing of events of the current connection!\n" 
				<< strerror(errno) 
				<< std::endl;
			return ;
		}
		
		// Idle
		if (!socketsWithData)
			continue;
		
		for (int index = 0; index < socketsWithData; index++)
		{
			if (events[index].data.fd == _listener_socket)
				handleUserConnection();
			else if (events[index].events & EPOLLHUP)
				handleUserDisconnection(events[index].data.fd);
			else
				handleUserData(events[index].data.fd);
		}
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
		handleUserDisconnection(it->second->getUserEPollEvent().data.fd);
	}

    for (std::map<std::string, User *>::const_iterator it = _authenticatedUsers.begin();
        it != _authenticatedUsers.end();
        it++)
	{
		if (!it->second)
			continue;

		it->second->sendMessage(message);
		handleUserDisconnection(it->second->getUserEPollEvent().data.fd);
	}

	cleanChannels();

	CommandManager *commandManager = CommandManager::getInstance();

    commandManager->cleanUp();

	delete commandManager;

    _connectedUsers.clear();
	_authenticatedUsers.clear();
	_sockets.clear();
	
	close(_listener_socket);
}