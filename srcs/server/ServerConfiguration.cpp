/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 00:03:40 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/24 11:29:16 by mbouthai         ###   ########.fr       */
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
#include <sys/epoll.h>

#include "Server.hpp"

void	Server::configure(void)
{
    // Creation socket for the server and check it's status 
    _listenerSocket = socket(AF_INET, SOCK_STREAM, 0);
    
	if (_listenerSocket < 0)
        (std::cerr << "Error : Creation of Socket Failed !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));
    {
        int enabled = 1;
        setsockopt(_listenerSocket, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled) );
    }
    
	// Set the socket to non-blocking mode
	if (fcntl(_listenerSocket, F_SETFL, O_NONBLOCK) < 0)
		(std::cerr << "Error : Unable to set Socket to non-blocking mode !" 
			<< std::endl,
			close(_listenerSocket), 
			std::exit(EXIT_FAILURE));

	sockaddr_in 			_address;
	socklen_t				_addressSize;

	_address.sin_family      = AF_INET;
    _address.sin_port        = htons(this->_port);
    _address.sin_addr.s_addr = INADDR_ANY;

	_addressSize = sizeof(_address);

	// Bind the socket to the IP address and port of the server and check the status 
    if (bind(_listenerSocket, reinterpret_cast<struct sockaddr *>(&_address), _addressSize) < 0)
        (std::cerr << "Error : Binding Failed due to: " 
			<< strerror(errno) 
			<< std::endl,
			close(_listenerSocket),
			std::exit(EXIT_FAILURE));

	 // Listening for incoming connection requests and check the status
    if (listen(_listenerSocket, SOMAXCONN) < 0)
	{
    	(std::cerr << "Error : Listening Failed !" 
			<< std::endl,
			close(_listenerSocket),
			std::exit(EXIT_FAILURE));
	}

	_epollInstance = epoll_create1(0);
	if (_epollInstance < 0)
	{
		(std::cerr << "Error : EPOLL Failed due to: " 
			<< strerror(errno) 
			<< std::endl,
			close(_listenerSocket),
			std::exit(EXIT_FAILURE));
	}
	
	//  Set the events member of poll_fd to monitor for input events
	struct epoll_event		_listenerEvent;
	
	_listenerEvent.events = EPOLLIN | EPOLLET;
	_listenerEvent.data.fd = _listenerSocket;
	
	if (epoll_ctl(_epollInstance, EPOLL_CTL_ADD, _listenerSocket, &_listenerEvent) < 0)
	{
		(std::cerr << "Error : EPOLL_CTL Failed due to: " 
			<< strerror(errno) 
			<< std::endl,
			close(_listenerSocket),
			std::exit(EXIT_FAILURE));
	}
}