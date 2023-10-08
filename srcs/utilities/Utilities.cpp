#include <cstring>
#include <algorithm>
#include <fstream>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>

#include "Server.hpp"
#include "CommandData.hpp"
#include "Utilities.hpp"

bool    validatePassword(const std::string& str)
{
    for (size_t index = 0; index < str.size(); index++)
    {
        if (std::isspace(str[index]))
            return (false);
    }
    return (true);
}

std::vector<pollfd>::iterator findSocket(std::vector<pollfd>& vec, int fd)
{
    for (std::vector<pollfd>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
        if (it->fd == fd)
            return (it);
    }
    return (vec.end());
}

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

std::string obtain_hostname(sockaddr_in& user_addr)
{
    std::string ret_str;
    char* user_ip;

    user_ip = inet_ntoa(user_addr.sin_addr);

    ret_str = user_ip;

    return ret_str;
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
        || command == "LIST"
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

bool	validateInput(const std::string& input)
{
	if (input.empty())
		return (false);

	bool flag = std::isspace(input[0]);
    bool trail = false;

	for (size_t index = 0; index < input.size(); index++)
	{
		if (std::isspace(input[index]) && !trail)
		{
			if (flag)
				return (false) ;
			flag = true;
		}
		else
        {
			flag = false;
            if (!trail && input[index] == ':' && index != 0)
                trail = true;
        }
	}
	
	if (!trail)
	{
	    return (!flag);
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

bool isValidIRCUsername(const std::string& username)
{
    // Check the length of the username
    if (username.empty() || username.size() > 50) {
        return false;
    }

    // Check the first character
    if (!(isalpha(username[0]) || username[0] == '_')) {
        return false;
    }

    // Check the rest of the characters
    for (size_t i = 1; i < username.size(); ++i) {
        if (!(isalnum(username[i]) || username[i] == '_' || username[i] == '-' || username[i] == '[' || username[i] == ']')) {
            return false;
        }
    }

    // All checks passed, the username is valid
    return true;
}

bool containsString(const std::set<std::string>& collection, const std::string& element)
{
    // Convert the provided string to lowercase for case-insensitive comparison
    std::string lowercaseElement = element;
    
    std::transform(lowercaseElement.begin(),
        lowercaseElement.end(),
        lowercaseElement.begin(), ::tolower);

    // Check if the lowercase element is in the given collection
    for (std::set<std::string>::const_iterator it = collection.begin();
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

    std::cout << "Valid: " << data.valid << std::endl;
}

Data emptyCommandData()
{
    std::vector<std::string> empty;

    Data data = {
        .prefix = "",
        .command = "",
        .arguments = empty,
        .trail = "",
        .originalInput = "",
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
        line += "Fun fact, u ain't got no girls";
        return (split(line, '\n'));
    }

    while (std::getline(motdFile, line))
        result.push_back(line);

    return (result);
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
				if (index + 1 < args.size())
				{
				    current.parameter = args[index + 1];
				}
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
				if (takesParam(args[index][j]) && args[index][0] == '+')
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
				
				if (takesParam(args[index][j]) && riz.add)
					riz.parameter = moreArgs[temp++];
				
				result.push_back(riz);
			}
			index += 2;
		}
	}

	return (result);
}

std::string getCurrentDateTime()
{
    std::time_t now = std::time(NULL);
    std::tm* tm_info = std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, 80, "%a %d %b %Z at %Y %I:%M:%S %p", tm_info);

    return buffer;
}
