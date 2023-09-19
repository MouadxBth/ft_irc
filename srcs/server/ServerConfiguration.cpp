/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfiguration.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 00:03:40 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 02:44:42 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cerrno>

#include "Server.hpp"

void	Server::configure(void)
{
    // Creation socket for the server and check it's status 
    _listener.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listener.fd < 0)
        (std::cerr << "Error : Creation of Socket Failed !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));
    {
		// Set the socket to non-blocking mode
        int enabled = 1;
        setsockopt (_listener.fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled) );
    }
    if (fcntl(_listener.fd, F_SETFL, O_NONBLOCK) < 0)
		(std::cerr << "Error : Unable to set Socket to non-blocking mode !" 
			<< std::endl, 
			std::exit(EXIT_FAILURE));

	_address.sin_family      = AF_INET;
    _address.sin_port        = htons(this->_port);
    _address.sin_addr.s_addr = INADDR_ANY;

	_addressSize = sizeof(_address);

	//  Set the events member of poll_fd to monitor for input and output events
    _listener.events  = POLLIN | POLLOUT;
    _listener.revents = 0;

	// Bind the socket to the IP address and port of the server and check the status 
    if (bind(_listener.fd, reinterpret_cast<struct sockaddr *>(&_address), _addressSize) < 0)
        (std::cerr << "Error : Binding Failed due to: " 
			<< strerror(errno) 
			<< std::endl, 
			std::exit(EXIT_FAILURE));

	 // Listening for incoming connection requests and check the status
    if (listen(_listener.fd, SOMAXCONN) < 0)
	{
    	(std::cerr << "Error : Listening Failed !" << std::endl, std::exit(EXIT_FAILURE));
	}

	_sockets.push_back(_listener);
}