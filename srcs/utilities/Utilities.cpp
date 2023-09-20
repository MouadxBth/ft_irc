/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utilities.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 01:07:58 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 01:20:11 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <algorithm>
#include <fstream>

#include "Server.hpp"
#include "Utilities.hpp"

std::string currentTimestamp(void)
{
    std::string result;
    std::time_t currentTime = std::time(NULL);

    struct std::tm* timeinfo = std::localtime(&currentTime);

    char timeString[80];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);

    result += "[";
    result += timeString;
    result += "]";
    return (result);
}

std::string obtain_hostname(sockaddr_in user_addr)
{
    std::string ret_str;
    struct addrinfo hint, *res;
    char *user_ip;

    user_ip = inet_ntoa(user_addr.sin_addr);

    bzero(&hint, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = user_addr.sin_family;
    
    getaddrinfo(user_ip, NULL, &hint, &res);

    if (res && res->ai_canonname)  
       ret_str.assign(res->ai_canonname);
    else
        ret_str.assign("EMPTY");
    freeaddrinfo(res);
    return (ret_str);
}

bool isKnownCommand(std::string command)
{
        /*|| command == "PING"
        || command == "LIST"
        || command == "BOT"
        || command == "INFO"*/
    if (command == "PASS"
        || command == "NICK"
        || command == "USER"
        || command == "JOIN"
        || command == "INVITE"
        || command == "PART"
        || command == "NOTICE"
        || command == "PRIVMSG"
        || command == "QUIT"
        || command == "MOTD"
        || command == "KICK"
        || command == "NAMES"
        || command == "TOPIC"
        || command == "MODE")
        return (true);
    return (false);
}

std::string trim(const std::string& str)
{
    if (str.length() < 2)
        return (str);
    size_t start = 0;
    size_t end = str.length() - 1;

    while (start <= end && std::isspace(str[start])) 
        start++;

    while (end >= start && std::isspace(str[end]))
        end--;

    return (str.substr(start, end - start + 1));
}

std::vector<std::string> split(std::string& input, char delimiter)
{
	std::vector<std::string> result;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter))
	{
		result.push_back(temp);
		temp.clear();
	}
	return (result);
}

void removeCharacter(std::string& str, char target) {
    size_t pos = 0;
    while ((pos = str.find(target, pos)) != std::string::npos) {
        str.erase(pos, 1);  // Erase 1 character at position 'pos'
    }
}

bool	validateInput(std::string& input)
{
	if (input.empty())
		return (false);

	bool flag = std::isspace(input[0]);

	for (size_t index = 0; index < input.size(); index++)
	{
		if (std::isspace(input[index]))
		{
			if (flag)
				return (false) ;
			flag = true;
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
		<< "\n\tArguments: ";
	
	for (std::vector<std::string>::iterator it = data.arguments.begin();
		it != data.arguments.end();
		it++)
	{
		std::cout << *it << " ";
	}
	
	std::cout << "\n\tArgument Size: " << data.arguments.size()
		<< "\n\tTrail: " << data.trail << " " << data.trail.size() << std::endl;
}

bool isNumber(const char *str)
{
    int index;

    index = -1;
    while (str[++index])
        if (!(str[index] >= '0' && str[index] <= '9'))
            return (false);
    if (!index)
        return (false);
    return (true);
}

size_t getNumber(const std::string& str)
{
    std::istringstream iss(str);
    long long result;

    iss >> result;

    if (!iss || iss.fail() || !iss.eof() || result < 0)
        return (0);
    
    return (static_cast<size_t>(result));
}

int validatePort(int port)
{
    // should think about limiting it to 6665 - 6667
	return (port > 1024 && port < 65537);
}

bool isValidIRCNickname(const std::string& nickname)
{
    // Check if the nickname is empty or too long
    // Check if the nickname starts with a letter
    if (nickname.empty() 
        || nickname.length() > 50
        || !std::isalpha(nickname[0]))
        return (false);

    // Check if the nickname contains only valid characters
    for (size_t index = 0; index < nickname.length(); ++index)
    {   
        if (!std::isalnum(nickname[index])
            && nickname[index] != '-' 
            && nickname[index] != '_' 
            && nickname[index] != '[' 
            && nickname[index] != ']' 
            && nickname[index] != '\\' 
            && nickname[index] != '`' 
            && nickname[index] != '^')
            return (false);
    }

    return (true);
}

bool containsString(const std::vector<std::string>& collection, const std::string& element)
{
    // Convert the provided string to lowercase for case-insensitive comparison
    std::string lowercaseElement = element;
    
    std::transform(lowercaseElement.begin(),
        lowercaseElement.end(),
        lowercaseElement.begin(), ::tolower);

    // Check if the lowercase element is in the given collection
    for (std::vector<std::string>::const_iterator it = collection.begin();
        it != collection.end();
        it++)
    {
        if (lowercaseElement == *it)
            return (true);
    }
    return false;
}

std::map<std::string, std::string> getElements(std::vector<std::string>& channels, std::vector<std::string>& passwords)
{
	std::map<std::string, std::string> elements;

    size_t index = 0;

	for (; index < passwords.size(); index++)
	{
		elements[channels[index]] = passwords[index];
	}

    while (index < channels.size())
    {
        elements[channels[index++]] = "";
    }

	return (elements);
}

void printDatas(Data &data)
{
	std::cout << "=>Command:" << "\n\tPrefix: " << data.prefix << " " << data.prefix.size()
		<< "\n\tCommand: " << data.command << " " << data.command.size()
		<< "\n\tArguments: ";
	
	for (std::vector<std::string>::iterator it = data.arguments.begin();
		it != data.arguments.end();
		it++)
	{
		std::cout << *it << " ";
	}
	
	std::cout << "\n\tArgument Size: " << data.arguments.size()
		<< "\n\tTrail: " << data.trail << " " << data.trail.size() << std::endl;
}

Data emptyCommandData()
{
    std::vector<std::string> empty;

    Data data = {
        .prefix = "",
        .command = "",
        .arguments = empty,
        .trail = "",
        .valid = true,
        .trailPresent = false
    };

    return (data);
}

std::vector<std::string> generateMotd()
{
    std::ifstream motdFile(".MOTD");
    std::string line;
    std::vector<std::string> result;

    if (!motdFile)
    {
        std::cout << "not found" << std::endl;
        line += "This is a dummy motd\n";
        line += "Fun fact, u ain't got no hoes";
        return (split(line, '\n'));
    }

    while (std::getline(motdFile, line))
        result.push_back(line);

    return (result);
}

bool wildcardMatch(const char* pattern, const char* text)
{
    if (*pattern == '\0')
        return (true);
    
    while (*text)
    {
        if (*pattern == *text || *pattern == '?')
        {
            ++pattern;
            ++text;
        }
        else if (*pattern == '*')
        {
            pattern++;

            // Handle special cases
            if (*pattern == '\0')
            {
                // If '*' is the last character in pattern, it matches the rest of 'text'.
                return (true);
            }
            else if (*pattern == '*')
            {
                // If there are consecutive '*', merge them into one
                while (*pattern == '*')
                    ++pattern;

                // Try each possible position in text
                while (*text)
                {
                    if (wildcardMatch(pattern, text))
                        return (true);
                    ++text;
                }
                return false;
            }
            else
            {
                // Normal '*' behavior, match any number of characters
                while (*text)
                {
                    if (wildcardMatch(pattern, text))
                        return (true);
                    ++text;
                }
                return (false);
            }
        }
        else
        {
            return (false);
        }
    }

    // Check if there are any remaining '*' in the pattern
    while (*pattern == '*')
        ++pattern;

    return (*pattern == '\0');
}

bool takesParam(int c)
{
	return (c == 'k' || c == 'o' || c == 'l');
}

std::vector<ChannelMode> parseModeArguments(std::vector<std::string>& args)
{
	std::vector<ChannelMode> result;
	std::vector<ChannelMode> empty;
	
	for (size_t index = 1; index < args.size();)
	{
		if (args[index][0] != '+' && args[index][0] != '-')
		{
			return (empty);
		}
			
		if (args[index].size() == 2)
		{
			ChannelMode current;

			current.add = args[index][0] == '+';
			current.mode = args[index][1];
			
			if (takesParam(current.mode))
			{
				if (index + 1 >= args.size())
				{
					return (empty);
				}
				current.parameter = args[index + 1];
				index++;
			}
			result.push_back(current);
			index++;
		}
		
		else if (args[index].size() > 2)
		{
			size_t takes = 0;

			for (size_t j = 1; j < args[index].size(); j++)
			{
				if (takesParam(args[index][j]))
					takes++;
			}
			
			if (!takes)
			{
				for (size_t j = 1; j < args[index].size(); j++)
				{
					ChannelMode riz;

					riz.add = args[index][0] == '+';
					riz.mode = args[index][j];
					result.push_back(riz);
				}
				index++;
				continue ;
			}
			
			std::vector<std::string> moreArgs;
			
			if (index + 1 >= args.size())
			{
				return (empty);
			}
			
			moreArgs = split(args[index + 1], ',');

			if (moreArgs.size() != takes)
			{
				return (empty);
			}

			for (size_t j = 1, temp = 0; j < args[index].size(); j++)
			{
				ChannelMode riz;

				riz.add = args[index][0] == '+';
				riz.mode = args[index][j];
				
				if (takesParam(args[index][j]))
					riz.parameter = moreArgs[temp++];
				
				result.push_back(riz);
			}
			index += 2;
		}
	}

	return (result);
}