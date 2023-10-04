#include "Channel.hpp"
#include <sys/socket.h>
#include <cstring>
#include <cerrno>

Channel::Channel()
	: _name("default"),
	_inviteOnly(false),
	_topicOperator(false),
	_key(false),
	_userLimit(false),
	_private(false),
	_moderated(false),
	_externalMessages(false)
{}

Channel::~Channel()
{
	// should not be possible
	for (std::map<std::string, std::pair<User *, ChannelUserModes> >::iterator it = _users.begin();
		it != _users.end();
		it++)
	{
		User *user = it->second.first;
		if (user)
			delete user;
	}

	_users.clear();
}

Channel::Channel(const Channel& instance) : _name(instance.getName())
{
	*this = instance;
}

Channel& Channel::operator=(const Channel& instance)
{
	if (this != &instance)
	{
		_users.clear();
		_inviteList.clear();
		_banList.clear();

		_users = instance.getUsers();
		_inviteList = instance.getInviteList();
		_banList = instance.getBanList();

		setInviteOnly(instance.isInviteOnly());
		setTopicSettableByChannelOperatorOnly(instance.isTopicSettableByChannelOperatorOnly());
		setTopic(instance.getTopic());
		
		setChannelKey(instance.isChannelKeySet());
		setPassword(instance.getPassword());

		setUserLimit(instance.isUserLimitSet());
		setMaximumCapacity(instance.getMaximumCapacity());

		setPrivate(instance.isPrivate());
		setModerated(instance.isModerated());
		setExternalMessagesEnabled(instance.isExternalMessagesEnabled());
	}
	return (*this);
}

Channel::Channel(const std::string& name)
	: _name(name),
	_inviteOnly(false),
	_topicOperator(false),
	_key(false),
	_userLimit(false),
	_private(false),
	_moderated(false),
	_externalMessages(false)
{}

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

const std::set<std::string>&	Channel::getInviteList() const
{
	return (this->_inviteList);
}

const std::set<std::string>&	Channel::getBanList() const
{
	return (this->_banList);
}

const std::map<std::string, std::pair<User *, ChannelUserModes> >& Channel::getUsers() const
{
	return (this->_users);
}

const std::pair<User *, ChannelUserModes>& Channel::getUser(const std::string& nickname) const
{
	return (_users.find(nickname)->second);
}

const std::string Channel::getUserNicknames() const
{
	std::string	nicknames;

	for (std::map<std::string, std::pair<User *, ChannelUserModes> >::const_iterator it = this->_users.begin(); 
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

const std::string Channel::getChannelUserModes() const
{
	std::string result("+");

	if (isTopicSettableByChannelOperatorOnly())
		result += "t";
	if (isInviteOnly())
		result += "i";
	if (isUserLimitSet())
		result += "l";
	if (isChannelKeySet())
		result += "k";
	
	if (isUserLimitSet())
	{
		result += " ";
		result += static_cast<int>(getMaximumCapacity());
	}
	
	if (isChannelKeySet())
		result += " " + getPassword();
	
	return (result);
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

bool Channel::isPrivate() const
{
	return (this->_private);
}

bool Channel::isModerated() const
{
	return (this->_moderated);
}

bool Channel::isExternalMessagesEnabled() const
{
	return (this->_externalMessages);
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

void Channel::setPrivate(bool _private)
{
	this->_private = _private;
}

void Channel::setModerated(bool moderated)
{
	this->_moderated = moderated;
}

void Channel::setExternalMessagesEnabled(bool externalMessages)
{
	this->_externalMessages = externalMessages;
}

bool Channel::isUserInvited(const std::string& nickname) const
{
	return (_inviteList.find(nickname) != _inviteList.end());
}

bool Channel::inviteUser(const std::string& nickname)
{
	return (this->_inviteList.insert(nickname).second);
}

bool Channel::unInviteUser(const std::string& nickname)
{
	return (this->_inviteList.erase(nickname));
}

bool Channel::isUserBanned(const std::string& nickname) const
{
	return (_banList.find(nickname) != _banList.end());
}

bool Channel::banUser(const std::string& nickname)
{
	return (this->_banList.insert(nickname).second);
}

bool Channel::pardonUser(const std::string& nickname)
{
	return (this->_banList.erase(nickname));
}

bool Channel::containsUser(const std::string& nickname) const
{
	const std::map<std::string, std::pair<User *, ChannelUserModes> >::const_iterator result = _users.find(nickname);
	return (result != _users.end() && result->second.first != NULL);
}

void Channel::addUser(User *user, ChannelUserModes& ChannelUserModes)
{
	this->_users[user->getNickname()] = std::make_pair(user, ChannelUserModes);
}
void Channel::removeUser(const std::string& nickname)
{
	this->_users.erase(nickname);
}

bool Channel::setOwner(const std::string& nickname, bool boolean)
{
	std::map<std::string, std::pair<User *, ChannelUserModes> >::iterator result = _users.find(nickname);

	if (result == _users.end() || !result->second.first)
		return (false);
	result->second.second.channelOwner = boolean;
	return (true);
}

bool Channel::setOperator(const std::string& nickname, bool boolean)
{
	std::map<std::string, std::pair<User *, ChannelUserModes> >::iterator result = _users.find(nickname);

	if (result == _users.end() || !result->second.first)
		return (false);
	result->second.second.channelOperator = boolean;
	return (true);
}

bool Channel::setVoice(const std::string& nickname, bool boolean)
{
	std::map<std::string, std::pair<User *, ChannelUserModes> >::iterator result = _users.find(nickname);

	if (result == _users.end() || !result->second.first)
		return (false);
	result->second.second.voice = boolean;
	return (true);
}

void Channel::broadcast(const std::string& nickname, std::string& message) const
{
	for (std::map<std::string, std::pair<User *, ChannelUserModes> >::const_iterator it = getUsers().begin();
		it != this->_users.end();
		it++)
	{
		if (it->second.first && it->second.first->getNickname() != nickname)
			it->second.first->sendMessage(message);
	}
}

void Channel::announce(const std::string& message) const
{
	for (std::map<std::string, std::pair<User *, ChannelUserModes> >::const_iterator it = getUsers().begin();
		it != this->_users.end();
		it++)
	{
		if (it->second.first)
			it->second.first->sendMessage(message);
	}
}

void Channel::announce(const std::vector<std::string>& messages) const
{
	for (std::vector<std::string>::const_iterator it = messages.begin();
		it != messages.end();
		it++)
	{
		announce(*it);
	}
}

std::ostream& operator<<(std::ostream& outputStream, const Channel& channel)
{
	outputStream << "Channel Info:\n"
		<< "=> Name: " << channel.getName()
		<< "\n=> Topic: " << channel.getTopic() 
		<< "\n=> Channel Key Set: " << (channel.isChannelKeySet() ? "Yes" : "No") << " Value: " << channel.getPassword()
		<< "\n=> User Limit Set: " << (channel.isUserLimitSet() ? "Yes" : "No") << " Value: " << channel.getMaximumCapacity()
		<< "\n=> Invite Only: " << (channel.isInviteOnly() ? "Yes" : "No")
		<< "\n=> Topic Control: " << (channel.isTopicSettableByChannelOperatorOnly() ? "Operator" : "All")
		<< "\n=> Private: " << (channel.isPrivate() ? "Yes" : "No")
		<< "\n=> Moderated: " << (channel.isModerated() ? "Yes" : "No")
		<< "\n=> External Messages Enabled: " << (channel.isExternalMessagesEnabled() ? "Yes" : "No")
		<< "\nInvite List:\n=> ";

    for (std::set<std::string>::const_iterator it = channel.getInviteList().begin();
		it != channel.getInviteList().end();)
    {
        outputStream << *it;
		it++;
		if (it != channel.getInviteList().end())
			outputStream << ", ";
    }

    outputStream << "\nBan List:\n=> ";

    for (std::set<std::string>::const_iterator it = channel.getBanList().begin();
		it != channel.getBanList().end();)
    {
        outputStream << *it;
		it++;
		if (it != channel.getBanList().end())
			outputStream << ", ";
    }

    outputStream << "\nUsers:"
		<< "\nTotal: " << channel.getUsers().size()
		<< "\n";

    for (std::map<std::string, std::pair<User *, ChannelUserModes> >::const_iterator it = channel.getUsers().begin();
		it != channel.getUsers().end();
		++it)
    {
        outputStream << " Nickname: " << it->first
			<< "\n\t- Channel Owner: " << (it->second.second.channelOwner ? "Yes" : "No")
			<< "\n\t- Channel Operator: " << (it->second.second.channelOperator ? "Yes" : "No")
			<< "\n\t- Has Voice: " << (it->second.second.voice ? "Yes" : "No");
    }

    return (outputStream);
}

