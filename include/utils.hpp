#include <iostream>
#include <ctime>
#include "Server.hpp"

std::string currentTimestamp(void);
std::string obtain_hostname(sockaddr_in user_addr);

bool isKnownCommand(std::string command);
