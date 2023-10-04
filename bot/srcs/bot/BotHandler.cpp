/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:58:50 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/03 20:26:28 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstring>
#include <cerrno>
#include <iostream>

#include "CommandReplies.hpp"

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

bool startsWith(const std::string& fullString, const std::string& startString) {
    // Check if the startString is longer than the fullString
    if (startString.length() > fullString.length()) {
        return false;
    }
    
    // Compare the startString to the beginning of the fullString
    return fullString.compare(0, startString.length(), startString) == 0;
}

// PRIVMSG bot !hello

bool    handleJoinReply(const std::string& reply)
{
    if (startsWith(reply, "332"))
        return (std::cout << ">> BOT JOINED CHANNEL SUCCESSFULLY << "
            << std::endl,
            false);
    else if (startsWith(reply, "403"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_NOSUCHCHANNEL << "
            << std::endl,
            false);
    else if (startsWith(reply, "405"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_TOOMANYCHANNELS << " 
            << std::endl,
            false);
    else if (startsWith(reply, "407"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_TOOMANYTARGETS << "
            << std::endl,
            false);
    else if (startsWith(reply, "437"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_UNAVAILRESOURCE << "
            << std::endl,
            false);
    else if (startsWith(reply, "461"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_NEEDMOREPARAMS << "
            << std::endl,
            false);
    else if (startsWith(reply, "471"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_CHANNELISFULL << "
            << std::endl,
            false);
    else if (startsWith(reply, "473"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_INVITEONLYCHAN << "
            << std::endl,
            false);
    else if (startsWith(reply, "474"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_BANNEDFROMCHAN << "
            << std::endl,
            false);
    else if (startsWith(reply, "475"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_BADCHANNELKEY << "
            << std::endl, false);
}

bool    handlePrivMsgReply(const std::string& reply)
{
	//ERR_NORECIPIENT() 411                ERR_NOTEXTTOSEND() 412
	//ERR_CANNOTSENDTOCHAN() 404
    //ERR_TOOMANYTARGETS() 407
	//ERR_NOSUCHNICK() 401
	//RPL_AWAY() 301

 	if (startsWith(reply, "332"))
        return (std::cout << ">> BOT JOINED CHANNEL SUCCESSFULLY << "
            << std::endl,
            false);
    else if (startsWith(reply, "403"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_NOSUCHCHANNEL << "
            << std::endl,
            false);
    else if (startsWith(reply, "405"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_TOOMANYCHANNELS << " 
            << std::endl,
            false);
    else if (startsWith(reply, "407"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_TOOMANYTARGETS << "
            << std::endl,
            false);
    else if (startsWith(reply, "437"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_UNAVAILRESOURCE << "
            << std::endl,
            false);
    else if (startsWith(reply, "461"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_NEEDMOREPARAMS << "
            << std::endl,
            false);
    else if (startsWith(reply, "471"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_CHANNELISFULL << "
            << std::endl,
            false);
    else if (startsWith(reply, "473"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_INVITEONLYCHAN << "
            << std::endl,
            false);
    else if (startsWith(reply, "474"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_BANNEDFROMCHAN << "
            << std::endl,
            false);
    else if (startsWith(reply, "475"))
        return (std::cout << ">> BOT COULD NOT JOIN CHANNEL: ERR_BADCHANNELKEY << "
            << std::endl,
            false);
}
bool checkNames(std::vector<std::string>::const_iterator it, std::string name)
{
    if (std::strcmp(name.c_str(), )
    
}
void    handleInput(std::vector<std::string>& input)
{
    //":" + user->getNickname() + "!" 
    //            + user->getUsername() + "@" 
    //            + user->getHostname() + " " 
    //            + data.command + " "
    std::string nickname;
    std::string message;
    
    for (std::vector<std::string>::const_iterator it = input.begin(); it != input.end(); it++)
    {
        if ((*it)[0] != ':')
            continue ;
        
        size_t index = 0;
        while ((*it)[++index] != '!')
            nickname += (*it)[index];
        //check username
        checkNames(it, this->getUsername())
        //check hostname
        checkNames(it, this->getHostname())
        
        //if (startsWith(*it, "475"))
    }
}