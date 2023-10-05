/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 23:58:13 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/05 16:17:14 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <cstring>
#include <cerrno>
#include <sstream>

#include "Server.hpp"
#include "CommandManager.hpp"
#include "CommandData.hpp"
#include "Utilities.hpp"

static std::string readUserInput(int fd)
{
	const int SIZE = 4096;
	char    buffer[SIZE];
	std::string result;
	ssize_t		bytesRead;

	memset(buffer,'\0', SIZE);

	bytesRead = recv(fd, buffer, SIZE, 0);
	
	if (bytesRead < 0)
	{
		// No more data to read at the moment
		//if (errno == EAGAIN || errno == EWOULDBLOCK)
		//	return (result);
		return (std::cerr << "Error : Failed to receive data from a socket descriptor!\n"
			<< strerror(errno)
			<< std::endl, result);
	}

	if (!bytesRead)
		return (result);

	result.append(buffer, bytesRead);
	
	return (result);
}

static std::vector<std::string> handleUserInput(User *user, std::string &input)
{
	std::vector<std::string> data;
	std::istringstream ss(input);
	
	std::string holder, temp;
	std::istringstream holderStream;
	
	while(std::getline(ss, holder) && !holder.empty())
	{
		// newline was not found
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
		
		if (holder.find('\r') == std::string::npos || holder[holder.size() - 1] != '\r')
		{
			user->setPartialMessage(holder);
			continue;
		}

		holder.erase(holder.end() - 1);

		removeCharacter(holder, char(4));

		if (!(holder.size() > 512))
			data.push_back(holder);

	}
	return (data);
}

static Data parseUserInput(std::string& input)
{
	std::vector<std::string> initializer;
	
	Data data = {
		.prefix = "",
		.command = "",
		.arguments = initializer,
		.trail = "",
		.originalInput = input,
		.valid = validateInput(input),
		.trailPresent = false
	};

	if (input.empty())
		return (data);

	size_t index = 0;
	size_t position = input.find_first_of(' ');
	
	if (input[0] == ':')
	{
		data.prefix = input.substr(1, position);
		index = position + 1;
	}

    if (std::isspace(input[index]))
    {
        data.valid = false;
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

static std::vector<Data> parseUserData(std::vector<std::string>& data)
{
	std::vector<Data> result;
	
	for (std::vector<std::string>::iterator it = data.begin(); it < data.end(); it++)
		result.push_back(parseUserInput(*it));

	return (result);
}

bool Server::handleUserData(int fd)
{
	User *user = getUser(fd);

	// unable to find user <-- has to be an error / user disconnected
	if (!user)
        return (false);

	std::string input = readUserInput(fd);

	//std::cout << "read: " << input << std::endl;

    // there is nothing to read from the client and the file descriptor is closed :
    if (input.empty())
		return (handleUserDisconnection(fd), false);

	std::vector<std::string> dataInput = handleUserInput(user, input);

	// handle user input
	std::vector<Data> data = parseUserData(dataInput);

	for (std::vector<Data>::iterator it = data.begin();
            it != data.end();
            it++)
		CommandManager::getInstance()->executeCommand(user, *it);

    return (true);
}