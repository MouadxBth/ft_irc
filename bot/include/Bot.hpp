/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:35:18 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/05 21:10:34 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <stdexcept>

typedef struct s_message
{
    std::string nickname;
    std::string username;
    std::string hostname;
    std::string command;
    std::string content;
}   Message;

class Bot
{

private:
    pollfd					_listener;
    sockaddr_in 			_serverAddress;

    bool                    _enabled;

    size_t                  _port;
    std::string             _password;
    std::string             _nickname;
    std::string             _username;
    std::string             _channel;
    std::string             _partialMessage;

    std::string                 readInput(pollfd& fd) throw(std::runtime_error);
    std::vector<std::string>    prepareInput(std::string& input);
    
    void                        sendMessage(const std::string& message);

public:
    Bot();
    ~Bot();
    Bot(const Bot& instance);
    Bot& operator=(const Bot& instance);

    Bot(std::string& password,
        std::string nickname = std::string("mawibot"),
        std::string username = std::string("Mawi Bot"),
        std::string channel = std::string("#bot"));

    void    configure(int port, std::string& address);

    void    establishConnection();

    void    destroyConnection();

    void    listen();

    Message parseMessage(const std::string& message);
    
    void    handleInput(std::vector<std::string>& input);
    bool    isKnownCommand(const std::string& command);
    void    executeMessage(const Message& message);


};