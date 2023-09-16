#include "../include/Channel.hpp"
#include <sys/socket.h>


Channel::Channel()
	: _inviteOnly(false),
	_private(false),
	_userLimit(false),
	_moderated(false),
	_externalMessages(false)
{}

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

const std::map<std::string, std::pair<User *, Modes> >& Channel::getUsers() const
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

bool Channel::isTopicSettableByChannelOperatorOnly() const
{
	return (this->_topicOperator);
}

bool Channel::isChannelKeySet() const
{
	return (this->_key);
}

bool Channel::isUserLimitSet() const
{
	return (this->_userLimit);
}

bool Channel::isExternalMessagesEnabled() const
{
	return (this->_externalMessages);
}

bool Channel::isModerated() const
{
	return (this->_moderated);
}

bool Channel::isInvited(const std::string& nickname) const
{
	return (_inviteList.find(nickname) != _inviteList.end());
}

bool Channel::isUserBanned(const std::string& nickname) const
{
	return (_banList.find(nickname) != _banList.end());
}

void Channel::setName(const std::string& name)
{
	this->_name = name;
}

void Channel::setTopic(const std::string& topic)
{
	this->_topic = topic;
}

void Channel::setPassword(const std::string& password)
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

void Channel::setTopicSettableByChannelOperatorOnly(bool topicOperator)
{
	this->_topicOperator = topicOperator;
}

void Channel::setChannelKey(bool _private)
{
	this->_key = _private;
}

void Channel::setUserLimit(bool _private)
{
	this->_userLimit = _private;
}

const std::string Channel::getUserNicknames() const
{
	std::string	nicknames;

	for (std::map<std::string, std::pair<User *, Modes> >::const_iterator it = this->_users.begin(); 
		it != this->_users.end();
		it++)
	{
		if (!nicknames.empty())
			nicknames += " ";
		if (it->second.second.channelOperator)
			nicknames += "@";
		nicknames += it->first;
	}
	return (nicknames);
}

const std::pair<User *, Modes>& Channel::getUser(const std::string& nickname) const
{
	return (_users.find(nickname)->second);
}

void Channel::addUser(User *user, bool isOperator, bool hasVoice)
{
	Modes modes = {
		.voice = hasVoice,
		.channelOperator = isOperator
	};
	this->_users[user->getNickname()] = std::make_pair(user, modes);
}
void Channel::removeUser(const std::string& nickname)
{
	this->_users.erase(nickname);
}

bool Channel::containsUser(const std::string& nickname) const
{
	return (_users.find(nickname) != _users.end() && getUser(nickname).first != NULL);
}

void Channel::promoteUser(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second.channelOperator = true;
}
void Channel::demoteUser(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second.channelOperator = false;
}

void Channel::grantVoice(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second.voice = true;
}
void Channel::revokeVoice(const std::string& nickname)
{
	if (containsUser(nickname))
		this->_users[nickname].second.voice = false;
}

void Channel::banUser(const std::string& nickname)
{
	this->_banList.insert(nickname);
}

void Channel::pardonUser(const std::string& nickname)
{
	this->_banList.erase(nickname);
}

void Channel::inviteUser(const std::string& nickname)
{
	this->_inviteList.insert(nickname);
}

void Channel::removeInvite(const std::string& nickname)
{
	_inviteList.erase(nickname);
}

void Channel::broadcast(const std::string& nickname, std::string& input) const
{
	std::string message = input;

	if (message[message.size() - 2] != '\r' && message[message.size() - 1] != '\n')
		message = input + "\r\n";

	for (std::map<std::string, std::pair<User *, Modes> >::const_iterator it = getUsers().begin();
		it != this->_users.end();
		it++)
	{
		if (it->second.first && it->second.first->getNickname() != nickname)
			send(it->second.first->getUserSocket().fd, message.c_str(), message.size(), 0);
	}
}

void Channel::announce(std::string& input) const
{
	std::string message = input;

	if (message[message.size() - 2] != '\r' && message[message.size() - 1] != '\n')
		message = input + "\r\n";
	
	for (std::map<std::string, std::pair<User *, Modes> >::const_iterator it = getUsers().begin();
		it != this->_users.end();
		it++)
	{
		if (it->second.first)
			send(it->second.first->getUserSocket().fd, message.c_str(), message.size(), 0);
	}
}

void Channel::announce(std::vector<std::string>& messages) const
{
	for (std::vector<std::string>::iterator it = messages.begin();
		it != messages.end();
		it++)
	{
		announce(*it);
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
		<< "\n=> Is Channel Key Set: " << channel.isChannelKeySet()
		<< "\n=> Is User Limit Set: " << channel.isUserLimitSet()
		<< "\n=> Members Count: " << channel.getUsers().size();
	return (outputStream);
}
