/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:58:50 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 23:14:56 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstring>
#include <cerrno>

std::string Bot::readInput(pollfd& connectionInfo) throw(std::runtime_error)
{
	const int SIZE = 4096;
	int		readData;
	char    buffer[SIZE];
	std::string result;
	
	memset(buffer,'\0', SIZE);
	
	readData = recv(connectionInfo.fd, buffer, SIZE - 1, 0);
	
	if (readData < 0)
	{
        std::string message = "Error : Failed to receive data from a socket descriptor!\n";
        
        message += strerror(errno);

        throw (std::runtime_error(message));
	}
	
	if (readData)
		result.append(buffer, readData);

	return (result);
}

std::vector<std::string> Bot::prepareInput(std::string& input)
{
    std::vector<std::string> result;
    
    size_t crlfPosition = input.find("\r\n");
    std::string temp;

    if (crlfPosition == std::string::npos)
    {
        _partialMessage += input;
        return (result);
    }

    std::string fullMessage = _partialMessage + input;

    _partialMessage.clear();

    crlfPosition = fullMessage.find("\r\n");

    while (crlfPosition != std::string::npos)
	{
		temp = fullMessage.substr(0, crlfPosition);
		fullMessage.erase(0, crlfPosition + 2);
		result.push_back(temp);

        crlfPosition = fullMessage.find("\r\n");
	}
    
    return (result);
}