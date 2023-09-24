/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 00:03:40 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 18:58:48 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include "Server.hpp"

void	Server::configure(void)
{
    // Creation socket for the server and check it's status 
    _listener.fd = socket(AF_INET, SOCK_STREAM, 0);
    _listener.events  = POLLIN;
    _listener.revents = 0;
	
	if (_listener.fd < 0)
        (std::cerr << "Error : Creation of Socket Failed !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));
    {
        int enabled = 1;
        setsockopt(_listener.fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled) );
    }
    
	// Set the socket to non-blocking mode
	if (fcntl(_listener.fd, F_SETFL, O_NONBLOCK) < 0)
		(std::cerr << "Error : Unable to set Socket to non-blocking mode !" 
			<< std::endl,
			close(_listener.fd), 
			std::exit(EXIT_FAILURE));

	sockaddr_in 			_address;
	socklen_t				_addressSize;

	_address.sin_family      = AF_INET;
    _address.sin_port        = htons(this->_port);
    _address.sin_addr.s_addr = INADDR_ANY;

	_addressSize = sizeof(_address);

	// Bind the socket to the IP address and port of the server and check the status 
    if (bind(_listener.fd, reinterpret_cast<struct sockaddr *>(&_address), _addressSize) < 0)
        (std::cerr << "Error : Binding Failed due to: " 
			<< strerror(errno) 
			<< std::endl,
			close(_listener.fd),
			std::exit(EXIT_FAILURE));

	 // Listening for incoming connection requests and check the status
    if (listen(_listener.fd, SOMAXCONN) < 0)
	{
    	(std::cerr << "Error : Listening Failed !" 
			<< std::endl,
			close(_listener.fd),
			std::exit(EXIT_FAILURE));
	}

	_sockets.push_back(_listener);

}