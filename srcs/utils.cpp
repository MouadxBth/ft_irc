#include "utils.hpp"

void print_timestamped(void)
{
    std::time_t currentTime = std::time(NULL);

    struct std::tm* timeinfo = std::localtime(&currentTime);

    char timeString[80];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);

    std::cout << "["  << timeString << "] ";
}

std::string &obtain_hostname(sockaddr_in user_addr)
{
    std::string *ret_str =  new std::string();
    struct addrinfo hint, *res;
    char *user_ip;

    user_ip = inet_ntoa(user_addr.sin_addr);

    bzero(&hint, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = user_addr.sin_family;
    
    getaddrinfo(user_ip, NULL, &hint, &res);

    if (res && res->ai_canonname)  
       ret_str->assign(res->ai_canonname);
    else
        ret_str->assign("EMPTY");
    freeaddrinfo(res);
    return (*ret_str);
}
