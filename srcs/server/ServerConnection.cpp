/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:56:31 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 12:03:16 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/ip.h>
#include <sys/socket.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "Server.hpp"
#include "CommandManager.hpp"
#include "CommandData.hpp"
#include "Utilities.hpp"

void Server::handleUserConnection()
{
	int newUserSocket;
	sockaddr_in newUserAddress;
	socklen_t newUserAddressSize = sizeof(newUserAddress);
	
	newUserSocket = accept(_listenerSocket,
		reinterpret_cast<struct sockaddr *>(&newUserAddress),
		&newUserAddressSize);
	
	if (newUserSocket < 0)
	{
		std::cerr << "Error : Unable to accept new connection !\n"
			<< strerror(errno)
			<< std::endl;
		return ;
	}

	//std::cout << "===> A User has connected to the server! <===" << std::endl;
	// set the newUserSocket to the non-blocking mode
	
	// UPDATE: i think this is not necessary
	if (fcntl(newUserSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : Accept Failed ! Unable to set user's file descriptor to non blocking mode.\n"
			<< strerror(errno)
			<< std::endl;
        close(newUserSocket);
		return ;
	}
	struct epoll_event  newUserEPollEvent;
	
	// fill the new pollfd struct of the newUser 
	newUserEPollEvent.data.fd      = newUserSocket;
	newUserEPollEvent.events  = EPOLLIN | EPOLLET;

	if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, newUserSocket, &newUserEPollEvent) < 0)
	{
		std::cerr << "Error : EPOLL_CTL Failed !\n"
			<< strerror(errno)
			<< std::endl;
        close(newUserSocket);
		return ;
	}

	// create a new user with fd
	User    *newUser = new User(newUserSocket);
	
	newUser->setHostname(obtain_hostname(newUserAddress));
	// add it to the Users map
	_connectedUsers[newUserSocket] = newUser;

	std::cout << "=> CONNECTED: [" << newUserSocket << "]" << std::endl;
}

void Server::handleUserDisconnection(int fd)
{
    User *user = getUser(fd);
    
    if (!user)
    {
        return ;
    }

	std::vector<std::string> empty;

	Data data = emptyCommandData();

	data.command = "QUIT";
	
	std::cout << "=> DISCONNECTED: [" << user->getSocket() << "]" << std::endl;

	CommandManager::getInstance()->executeCommand(user, data);

}