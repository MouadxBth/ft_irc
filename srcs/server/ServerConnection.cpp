/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:56:31 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 02:14:45 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cerrno>

#include "Server.hpp"
#include "CommandManager.hpp"

// accept the new connection and create newUserSocket
// since the server socket is listening in non blocking mode, the accepted socket (new user socket)
// will also inherit the non blocking mode
void Server::handleUserConnection()
{
	int newUserSocket;
	sockaddr_in newUserAddress;
	socklen_t newUserAddressSize = sizeof(newUserAddress);
	
	newUserSocket = accept(_listener.fd,
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
	if (fcntl(newUserSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : Accept Failed ! Unable to set user's file descriptor to non blocking mode.\n"
			<< strerror(errno)
			<< std::endl;
        close(newUserSocket);
		return ;
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
	newUser->setUserSocket(newUserFd);
	// add it to the Users map
	_connectedUsers[newUserFd.fd] = newUser;

	//std::cout << "\nNew user: " << *newUser << "\n" << std::endl;

    _sockets.push_back(newUserFd);
}

void Server::handleUserDisconnection(const pollfd& connectionInfo)
{
    User *user = getUser(connectionInfo.fd);
    
    if (!user)
    {
        return ;
    }

	std::vector<std::string> empty;

	Data data = {
		.prefix = "",
		.command = "QUIT",
		.arguments = empty,
		.trail = "",
		.valid = true,
		.trailPresent = false
	};

	CommandManager::getInstance()->executeCommand(user, data);
}