/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:43:46 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/20 18:14:32 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void Bot::sendMessage(const std::string& input) {
    std::string message = input;
    
    if (message[message.size() - 2] != '\r' && message[message.size() - 1])
        message += "\r\n";
        
    send(_listener.fd, message.c_str(), message.length(), 0);
}
