#pragma once

#include <iostream>
#include <ctime>

#include "ChannelMode.hpp"

std::string currentTimestamp(void);
std::string obtain_hostname(sockaddr_in user_addr);

bool isKnownCommand(std::string command);

std::vector<std::string> split(std::string& input, char delimiter);
std::string trim(const std::string& str);

void    removeCharacter(std::string& str, char target);
bool	validateInput(std::string& input);

bool isNumber(const char *str);

size_t getNumber(const std::string& str);

int validatePort(int port);

bool isValidIRCNickname(const std::string& nickname);

bool containsString(const std::vector<std::string>& collection, const std::string& element);

std::map<std::string, std::string> getElements(std::vector<std::string>& channels, std::vector<std::string>& passwords);
void printDatas(Data &data);

Data    emptyCommandData();

std::vector<std::string> generateMotd();

bool wildcardMatch(const char* pattern, const char* text);

bool takesParam(int c);

std::vector<ChannelMode> parseModeArguments(std::vector<std::string>& args);

std::string getCurrentDateTime();
