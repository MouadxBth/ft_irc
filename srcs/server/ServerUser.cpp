/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:24:57 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/11 22:02:43 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <../include/utils.hpp>
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <cerrno>

void    Server::removeSocket(pollfd& socket)
{
    for (std::vector<pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
    {
        if (it->fd == socket.fd)
        {
            it = _sockets.erase(it);
            break ;
        }
    }
}

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

	std::cout << "===> A User has joined the server! <===" << std::endl;
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
	_users[newUserFd.fd] = newUser;

	std::cout << "\nNew user: " << *newUser << "\n" << std::endl;

    _sockets.push_back(newUserFd);
}

void Server::handleUserDisconnect(const pollfd& connectionInfo)
{
    const User *user = getUser(connectionInfo.fd);
	
    std::cout << currentTimestamp() << " ";

    if (user)
    {
        std::cout << "User: [" << connectionInfo.fd << "] " 
            << user->getUsername() 
            << " has disconnected" 
            << std::endl;
    }
    else
    {
        std::cout << "A User: has disconnected" << std::endl;
    }

	close(connectionInfo.fd);
	_users.erase(connectionInfo.fd);

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

std::string Server::readUserInput(pollfd& connectionInfo)
{
	int		readData;
	char    buffer[4096];
	
	memset(buffer,'\0', 4096);
	
	// set recv on non blocking mode
	readData = recv(connectionInfo.fd, buffer, 4096, 0);
	
	std::string result(buffer);

	if (readData < 0)
		return (std::cerr << "Error : Failed to receive data from a socket descriptor!\n"
			<< strerror(errno)
			<< std::endl, result);	// std::exit(EXIT_FAILURE); or just return

	return (result);
}

std::vector<std::string> Server::handleUserInput(User *user, std::string &input)
{
	std::vector<std::string> data;
	std::stringstream ss(input);
	std::string holder;
	

	while(std::getline(ss, holder) && !holder.empty())
	{
		if (!user->getPartialMessage().empty())
		{
			holder = user->getPartialMessage() + holder;
			user->setPartialMessage("");
		}
		
		if (*(holder.end() - 1) != '\r')
		{
			holder += "\n";
			user->setPartialMessage(holder);
			continue;
		}

		holder.erase(holder.end() - 1);
		data.push_back(holder);
	}
	return (data);
}

Data Server::parseUserInput(std::string& input)
{
	Data data;
    std::stringstream ss(input);

    if (input[0] == ':')
        ss >> data.prefix;
    
    ss >> data.command;

    std::string temp;

    while ((ss >> temp) && temp[0] != ':')
    {
        data.arguments += temp;
        temp.clear();
    }

    if (temp[0] == ':')
    {
        temp.clear();
        std::getline(ss, temp);
        data.trail += temp;
    }
    return (data);
}

void printData(Data &data)
{
	std::cout << "Prefix: " << data.prefix << " " << data.prefix.size()
		<< "\nCommand: " << data.command << " " << data.command.size()
		<< "\nArguments: " << data.arguments << " " << data.arguments.size()
		<< "\nTrail: " << data.trail << " " << data.trail.size() << std::endl;
}

std::vector<Data> Server::parseUserData(std::vector<std::string>& data)
{
	std::vector<Data> result;

	for (std::vector<std::string>::iterator it = data.begin(); it < data.end(); it++)
		result.push_back(parseUserInput(*it));

	for (std::vector<Data>::iterator it = result.begin(); it < result.end(); it++)
		printData(*it);
	return (result);
}

/*
void execute(User *user, std::vector<Data>& data)
{
	for (std::vector<Data>::const_iterator it = data.begin(); it != data.end(); it++)
	{
		if (!isKnownCommand(it->command))
		{
			
		}
	}
}*/

bool Server::handleUserData(pollfd& connectionInfo)
{
	User *user = _users.find(connectionInfo.fd)->second;
    bool result = true;

	// unable to find user <-- has to be an error / user disconnected
	if (!user)
        return (false);

	std::string input = readUserInput(connectionInfo);	

    // there is nothing to read from the client and the file descriptor is closed :
    if (input.empty())
    {
        result = false;
		handleUserDisconnect(connectionInfo);
    }

	std::vector<std::string> dataInput = handleUserInput(user, input);

	// handle user input
	std::vector<Data> data = parseUserData(dataInput);
    return (result);
}