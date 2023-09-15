/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:24:57 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/15 13:42:06 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstring>

#include "Server.hpp"
#include "CommandManager.hpp"
#include "utils.hpp"

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
        std::cout << "User: [" << connectionInfo.fd << "] ";
		if (!user->getUsername().empty())
            std::cout << user->getUsername() << " ";
        std::cout << "has disconnected" 
            << std::endl;
    }
    else
    {
        std::cout << "A User: has disconnected" << std::endl;
    }

	close(connectionInfo.fd);

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

// Line 1\n Line2\r\n Line 3\n
// Line 1
// Line 2

// Line 3

void    printAscis(const std::string& str)
{
    for (size_t index = 0; index < str.size(); index++)
    {
        std::cout << ((int)str[index]) << " ";
    }
    std::cout << std::endl;
}

std::string Server::readUserInput(pollfd& connectionInfo)
{
	const int SIZE = 4096;
	int		readData;
	char    buffer[SIZE];
	std::string result;
	
	memset(buffer,'\0', SIZE);
	
	readData = recv(connectionInfo.fd, buffer, SIZE - 1, 0);
	
	if (readData < 0)
	{
		return (std::cerr << "Error : Failed to receive data from a socket descriptor!\n"
			<< strerror(errno)
			<< std::endl, result);
	}
	
	if (readData)
		result.append(buffer, readData);

	std::cout << result << std::endl;
	printAscis(result);

	return (result);
}

void removeCharacter(std::string& str, char target) {
    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::string::npos) {
        str.erase(pos, 1);  // Erase 1 character at position 'pos'
    }
}

std::vector<std::string> Server::handleUserInput(User *user, std::string &input)
{
	std::vector<std::string> data;
	std::istringstream ss(input);
	
	std::string holder, temp;
	std::istringstream holderStream;
	
	// 65 32 65
	// 65 13 65
	// 65 32 65 13 65
	// 
	while(std::getline(ss, holder) && !holder.empty())
	{
		// newline was not found (should be impossible but nevertheless)
		if (ss.eof())
		{
			holder = user->getPartialMessage() + holder;
			user->setPartialMessage(holder);
			continue ;
		}

		// set holder to holder + partial message
		if (!user->getPartialMessage().empty())
		{
			holder = user->getPartialMessage() + holder;
			user->setPartialMessage("");
		}
		
		if (holder.find('\r') == std::string::npos)
		{
			user->setPartialMessage(holder);
			continue;
		}

		holder.erase(holder.end() - 1);

		removeCharacter(holder, char(4));

		data.push_back(holder);

	}

	std::cout << "Data size: " << data.size() << std::endl;

	return (data);
}

bool	validateInput(std::string& input)
{
	if (input.empty())
	{
		std::cout << "Inv due to empty input" << std::endl;
		return (false);
	}

	bool flag = std::isspace(input[0]);

	for (size_t index = 0; index < input.size(); index++)
	{
		if (std::isspace(input[index]))
		{
			if (flag)
			{
				std::cout << "Inv due to space at start" << std::endl;
				return (false) ;
			}
			flag = true;
		}
		else if (input[index] == ':' 
			&& index + 1 < input.size() 
			&& std::isspace(input[index + 1]))
		{
			std::cout << "Inv due to space directly after : " << std::endl;
			return (false) ;
		}
		else
			flag = false;
	}

	return (true);
}

void printData(Data &data)
{
	std::cout << "=>Command:" << "\n\tPrefix: " << data.prefix << " " << data.prefix.size()
		<< "\n\tCommand: " << data.command << " " << data.command.size()
		<< "\n\tArguments: " << data.arguments.size()
		<< "\n\tTrail: " << data.trail << " " << data.trail.size() << std::endl;
}

Data Server::parseUserInput(User *user, std::string& input)
{
	std::vector<std::string> initializer;
	
	Data data = {
		.prefix = "",
		.command = "",
		.arguments = initializer,
		.trail = "",
		.simultaneousNickname = std::make_pair(false, user),
		.valid = validateInput(input),
		.trailPresent = false
	};

	if (input.empty())
		return (data);

	size_t index = 0;
	size_t position = input.find_first_of(' ');
	
	if (input[0] == ':')
	{
		data.command = input.substr(1, position);
		index = position + 1;
	}

	bool command = true;
	std::string arguments;

	for (; index < input.size(); index++)
	{
		// trail
		if (input[index] == ':')
		{
			data.trailPresent = true;
			while (++index < input.size())
				data.trail += input[index];
			break ;
		}
		if (input[index] == ' ' && command)
		{
			command = false;
			continue;
		}
		if (command)
			data.command += input[index];
		else
			arguments += input[index];
	}

	arguments = trim(arguments);
	data.arguments = split(arguments, ' ');
    return (data);
}

// nick one <--
// nick two <-- 
// nick one <-- same
// nick one <-- same
// nick one <-- same

std::vector<Data> Server::parseUserData(User *user, std::vector<std::string>& data)
{
	std::vector<Data> result;
	
	for (std::vector<std::string>::iterator it = data.begin(); it < data.end(); it++)
		result.push_back(parseUserInput(user, *it));
	

	return (result);
}

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
		_users.erase(connectionInfo.fd);
    }

	std::vector<std::string> dataInput = handleUserInput(user, input);

	// handle user input
	std::vector<Data> data = parseUserData(user, dataInput);

	if (data.size())
		user->setParsedData(data);

	std::cout << "Commands: " << data.size() << std::endl;

	
    return (result);
}