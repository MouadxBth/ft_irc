
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <arpa/inet.h>

# include "Bot.hpp"

void Bot::configure(int port, std::string& address)
{
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

    _listener.events  = POLLIN;
    _listener.revents = 0;
    
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_port = htons(port);
    _serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
}
