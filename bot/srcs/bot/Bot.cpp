/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:43:46 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 23:14:08 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot()
{

}

Bot::~Bot()
{
    
}

Bot::Bot(const Bot& instance)
{
    (void) instance;
}

Bot& Bot::operator=(const Bot& instance)
{
    (void) instance;
    return (*this);
}

Bot::Bot(std::string& password,
        std::string nickname,
        std::string username,
        std::string channel)
{
    _password = password;
    _nickname = nickname;
    _username = username;
    _channel = channel;
}

void Bot::sendMessage(const std::string& input) {
    std::string message = input;
    
    if (message[message.size() - 2] != '\r' && message[message.size() - 1])
        message += "\r\n";
        
    send(_listener.fd, message.c_str(), message.length(), 0);
}
