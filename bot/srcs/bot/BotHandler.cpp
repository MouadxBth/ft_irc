
#include "Bot.hpp"
#include <cstring>
#include <cerrno>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

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

Message Bot::parseMessage(const std::string& message) {
    Message result;
    std::istringstream iss(message);

    std::string myname;
    
    char colon;
    
    iss >> colon;
    
    std::getline(iss, result.nickname, '!');
    
    std::getline(iss, result.username, '@');
        
    std::getline(iss, result.hostname, ' ');
    
    std::getline(iss, result.command, ' ');

    std::getline(iss, myname, ' ');
    
    std::getline(iss, result.content);
    
    result.content.erase(0, 1);

    return (result);
}


void    Bot::handleInput(std::vector<std::string>& input)
{
    Message current;
    
    for (std::vector<std::string>::const_iterator it = input.begin(); it != input.end(); it++)
    {
        if ((*it)[0] != ':')
            continue ;
        current = parseMessage(*it);
        if (current.command.empty() || current.nickname.empty() || current.content.empty())
            continue;
        executeMessage(current);
    }
}


bool    Bot::isKnownCommand(const std::string& command)
{
    return (command == "!flip" 
        || command == "!random" 
        || command == "!who"
        || command == "!help");
}


void    Bot::executeMessage(const Message& message)
{
    if (message.command != "PRIVMSG" && message.command != "NOTICE")
        return ;
    
    if (!isKnownCommand(message.content))
        return ;

    if (message.content == "!flip")
    {
        std::string result = std::rand() % 2 ? "heads" : "tails";
        sendMessage("PRIVMSG " + message.nickname + " :" + result);
        return ;
    }
    
    if (message.content == "!random")
    {
        std::stringstream iss;

        iss << (std::rand() % 1000);
        
        sendMessage("PRIVMSG " + message.nickname + " :" + iss.str());
        return ;
    }

    if (message.content == "!who")
    {
        sendMessage("PRIVMSG " + message.nickname + " :Hello! my name is mawibot! i'm a Bot!");
        return ;
    }

    if (message.content == "!help")
    {
        sendMessage("PRIVMSG " + message.nickname + " :Here are the available commands:");
        sendMessage("PRIVMSG " + message.nickname + " :!flip (Flipping a coin, will return either a heads or tails)");
        sendMessage("PRIVMSG " + message.nickname + " :!random (Will generate a random number in this range [0-1000])");
        sendMessage("PRIVMSG " + message.nickname + " :!who (Presents the bot information)");
        sendMessage("PRIVMSG " + message.nickname + " :!help (To showcase the available commands)");
        return ;
    }
}
