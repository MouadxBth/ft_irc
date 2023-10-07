#include <netinet/ip.h>
#include <sys/socket.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "Server.hpp"
#include "CommandManager.hpp"
#include "CommandData.hpp"
#include "Utilities.hpp"

void Server::handleUserConnection()
{
	int newUserSocket;
	sockaddr_in newUserAddress;
	socklen_t newUserAddressSize = sizeof(newUserAddress);
	
	newUserSocket = accept(_listener.fd,
		reinterpret_cast<struct sockaddr *>(&newUserAddress),
		&newUserAddressSize);
	
	if (newUserSocket < 0)
	{
		std::cerr << "Error : Unable to accept new connection !\n"
			<< strerror(errno)
			<< std::endl;
		return ;
	}

	//std::cout << "===> A User has connected to the server! <===" << std::endl;
	// set the newUserSocket to the non-blocking mode
	
	// UPDATE: i think this is not necessary
	if (fcntl(newUserSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error : Accept Failed ! Unable to set user's file descriptor to non blocking mode.\n"
			<< strerror(errno)
			<< std::endl;
        close(newUserSocket);
		return ;
	}
	
	pollfd  newUserPollFd;
	
	// fill the new pollfd struct of the newUser 
	newUserPollFd.fd      = newUserSocket;
	newUserPollFd.events  = POLLIN | POLLOUT;

	// create a new user with fd
	User    *newUser = new User(newUserPollFd);
	
	newUser->setHostname(obtain_hostname(newUserAddress));

	// add it to the Users map
	_connectedUsers[newUserSocket] = newUser;

	_sockets.push_back(newUserPollFd);

	std::cout << "=> CONNECTED: [" << newUserSocket << "]" << std::endl;
}

void Server::handleUserDisconnection(int fd)
{
    User *user = getUser(fd);
    
    if (!user)
    {
        return ;
    }

	std::vector<std::string> empty;
	
	Data data = emptyCommandData();

	data.command = "QUIT";
	
	std::cout << "=> DISCONNECTED: [" << user->getSocket().fd << "]" << std::endl;

	CommandManager::getInstance()->executeCommand(user, data);

}