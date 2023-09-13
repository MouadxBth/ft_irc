#include "utils.hpp"
#include <strings.h>

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
    if (command == "PASS"
        || command == "NICK"
        || command == "USER"
        || command == "PING"
        || command == "JOIN"
        || command == "INVITE"
        || command == "PART"
        || command == "NOTICE"
        || command == "PRIVMSG"
        || command == "QUIT"
        || command == "KICK"
        || command == "NAMES"
        || command == "LIST"
        || command == "TOPIC"
        || command == "MODE"
        || command == "BOT"
        || command == "INFO")
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