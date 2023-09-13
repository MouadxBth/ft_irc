#include "../include/Channel.hpp"
#include <sys/socket.h>

Channel::Channel() : _inviteOnly(false), _private(false){}

Channel::~Channel() {}

Channel::Channel(const Channel& instance)
{
	(void) instance;
}

Channel& Channel::operator=(const Channel& instance)
{
	(void) instance;
	return (*this);
}

const std::string& Channel::getName() const
{
	return (this->_name);
}

const std::string& Channel::getTopic() const
{
	return (this->_topic);
}

const std::string& Channel::getPassword() const
{
	return (this->_password);
}

size_t Channel::getMaximumCapacity() const
{
	return (this->_maximumCapacity);
}

const std::map<std::string, std::pair<User *, bool> >& Channel::getUsers() const
{
	return (this->_users);
}

bool Channel::isPrivate() const
{
	return (this->_private);
}

bool Channel::isInviteOnly() const
{
	return (this->_inviteOnly);
}

bool Channel::isTopicOperator() const
{
	return (this->_topicOperator);
}


void Channel::setName(std::string& name)
{
	this->_name = name;
}

void Channel::setTopic(std::string& topic)
{
	this->_topic = topic;
}

void Channel::setPassword(std::string& password)
{
	this->_password = password;
}

void Channel::setMaximumCapacity(size_t capacity)
{
	this->_maximumCapacity = capacity;
}

void Channel::setInviteOnly(bool inviteOnly)
{
	this->_inviteOnly = inviteOnly;
}

void Channel::setPrivate(bool _private)
{
	this->_private = _private;
}

void Channel::setTopicOperator(bool topicOperator)
{
	this->_topicOperator = topicOperator;
}

const std::string Channel::getUserNicknames() const
{
	std::string	nicknames;

	for (std::map<std::string, std::pair<User *, bool> >::const_iterator it = this->_users.begin(); 
		it != this->_users.end();
		it++)
	{
		if (!nicknames.empty())
			nicknames += " ";
		if (it->second.second)
			nicknames += "@";
		nicknames += it->first;
	}
	return (nicknames);
}

const std::pair<User *, bool>& Channel::getUser(const std::string& nickname) const
{
	return (this->_users.find(nickname)->second);
}

void Channel::addUser(User *user, bool isOperator)
{
	this->_users[user->getNickname()] = std::make_pair(user, isOperator);
}
void Channel::removeUser(const std::string& nickname)
{
	this->_users.erase(nickname);
}

bool Channel::containsUser(const std::string& nickname) const
{
	return (getUser(nickname).first != NULL);
}

void Channel::promoteUser(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second = true;
}
void Channel::demoteUser(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second = false;
}

void Channel::broadcast(const std::string& nickname, std::string& message) const
{
	std::string constructedMessage(nickname + ": " + message);

	announce(constructedMessage);
}

void Channel::announce(std::string& message) const
{
	for (std::map<std::string, std::pair<User *, bool> >::const_iterator it = getUsers().begin();
		it != this->_users.end();
		it++)
	{
		if (it->second.first)
			send(it->second.first->getUserSocket().fd, message.c_str(), message.size(), 0);
	}
}

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel)
{
	outputStream << "Channel Info:\n" 
		<< "=> Name: " << channel.getName() 
		<< "\n=> Topic: " << channel.getTopic() 
		<< "\n=> Maximum Capacity: " << channel.getMaximumCapacity()
		<< "\n=> Is Invite Only: " <<channel.isInviteOnly()
		<< "\n=> Is Private: " << channel.isPrivate()
		<< "\n=> Members Count: " << channel.getUsers().size();
	return (outputStream);
}
