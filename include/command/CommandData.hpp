
#pragma once

#include <string>
#include <vector>

class User;

typedef struct s_data
{
    std::string prefix;
    std::string command;
    std::vector<std::string> arguments;
    std::string trail;
    std::string originalInput;
	bool valid;
    bool trailPresent;
} Data;
