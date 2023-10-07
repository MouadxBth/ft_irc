
#include <unistd.h>

#include "Server.hpp"

void	Server::cleanConnectedUsers()
{
	for (std::map<int, User *>::const_iterator it = _connectedUsers.begin();
        it != _connectedUsers.end();
        it++)
	{
		if (!it->second)
			continue;

		std::string nickname = it->second->getNickname().empty() 
			? "*" 
			: it->second->getNickname();

		std::string message = "ERROR :Closing Link: " 
			+ it->second->getHostname() 
			+ " (Quit: " + nickname + ")";

		it->second->sendMessage(message);

		close(it->second->getSocket().fd);

		delete it->second;	
	}

    _connectedUsers.clear();
}

void	Server::cleanAuthenticatedUsers()
{
	for (std::map<std::string, User *>::const_iterator it = _authenticatedUsers.begin();
        it != _authenticatedUsers.end();
        it++)
	{
		if (!it->second)
			continue;
			
		std::string reply = ":" + it->second->getNickname() + "!" 
                + it->second->getUsername() + "@" 
                + it->second->getHostname() + " QUIT :QUIT: " + it->second->getNickname();

		for (std::map<std::string, Channel *>::const_iterator ct = Server::getInstance()->getChannels().begin();
			ct != Server::getInstance()->getChannels().end();
			ct++)
		{
			if (!ct->second || !ct->second->containsUser(it->second->getNickname()))
				continue ;

			ct->second->broadcast(it->second->getNickname(), reply);
			ct->second->removeUser(it->second->getNickname());
			
			if (!ct->second->getUsers().size())
			{
				std::cout << "Staged channel: " << ct->first << std::endl;
				Server::getInstance()->removeChannel(ct->first);
			}
		}

		std::string message = "ERROR :Closing Link: " 
			+ it->second->getHostname() 
			+ " (Quit: " + it->second->getNickname() + ")";

		it->second->sendMessage(message);

		close(it->second->getSocket().fd);

		delete it->second;	
	}

    _authenticatedUsers.clear();
}

void	Server::cleanChannels()
{
	Channel *target;
    
	for (std::set<std::string>::iterator it = _channelsToBeRemoved.begin();
        it != _channelsToBeRemoved.end();
		it++)
	{
        target = getChannel(*it);
        if (target && !target->getUsers().size())
        {
			std::cout << "deleting channel: " << target->getName() << std::endl;
            _channels.erase(*it);
            delete target;
        }
	}

    _channelsToBeRemoved.clear();
    //_channels.clear();
}