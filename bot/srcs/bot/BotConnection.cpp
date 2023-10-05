/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:57:23 by mbouthai          #+#    #+#             */
/*   Updated: 2023/10/05 19:57:51 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdlib.h>

# include "Bot.hpp"

void Bot::establishConnection()
{
    int retries = 0;

    int result = 0;
    
    while (true)
    {
        result = connect(_listener.fd,    
                reinterpret_cast<struct sockaddr *>(&_serverAddress),
                sizeof(_serverAddress));
        if (!result)
            break ;

        if (errno != ECONNREFUSED)
            continue ;
            
        if (++retries > 3)
        {
            std::cerr << "Error: Unable to connect to server. Exhausted all retries!" << std::endl;
            close(_listener.fd);
            std::exit(1) ;   
        }
        
        std::cerr << "Error: Unable to connect to server.\n"
            << strerror(errno) 
            << std::endl;
        
        std::cout << retries << ") Retrying in 5 seconds..." << std::endl;
        sleep(5);
    }

    sendMessage("PASS " + _password);
    sendMessage("NICK " + _nickname);
    sendMessage("USER " + _username + " 0 * :" + _username + " Bot");
}

void Bot::destroyConnection()
{
    sendMessage("QUIT :" + _username + " has been terminated.");
	close(_listener.fd);
}