#include "User.hpp"
#include <vector>
#include <cstring>
#include <cerrno>

User::User()
	: _authenticated(false), 
	_operator(false),
	_away(false),
	_usedPassword(false),
	_joinedChannelsCount(0)
{}

User::~User() {}

User::User(const User& instance)
{
	*this = instance;
}

User& User::operator=(const User& instance)
{
	if (this != &instance)
	{
		setUsername(instance.getUsername());
		setFullname(instance.getFullname());
		setHostname(instance.getHostname());
		setNickname(instance.getNickname());
		setPassword(instance.getPassword());
		setPartialMessage(instance.getPartialMessage());
		setUserEPollEvent(instance.getUserEPollEvent());
		setAddress(instance.getAddress());
		setAuthenticated(instance.isAuthenticated());
		setOperator(instance.isOperator());
		setJoinedChannelsCount(instance.getJoinedChannelsCount());
	}
	return (*this);
}

const std::string&   User::getUsername() const
{
	return (this->_username);
}

const std::string&    User::getNickname() const
{
	return (this->_nickname);
}

const std::string&    User::getFullname() const
{
	return (this->_fullname);
}

const std::string&    User::getHostname() const
{
	return (this->_hostname);
}

const std::string&    User::getPassword() const
{
	return (this->_password);
}

const std::string&    User::getPartialMessage() const
{
	return (this->_partialMessage);
}

const std::string&	User::getAwayMessage() const
{
	return (this->_awayMessage);
}

bool            User::isAuthenticated() const
{
	return (this->_authenticated);
}

bool            User::isOperator() const
{
	return (this->_operator);
}

bool            User::isAway() const
{
	return (this->_away);
}

const epoll_event&    User::getUserEPollEvent() const
{
	return (this->_userEPollEvent);
}

const sockaddr_in&    User::getAddress() const
{
	return (_address);
}

bool	User::hasUsedPassword() const
{
	return (_usedPassword);
}

int	User::getJoinedChannelsCount() const
{
	return (_joinedChannelsCount);
}

void   User::setUsername(const std::string& username)
{
	this->_username = username;
}

void   User::setNickname(const std::string& nickname)
{
	this->_nickname = nickname;
}

void    User::setFullname(const std::string& fullname)
{
	this->_fullname = fullname;
}

void    User::setHostname(const std::string& hostname)
{
	this->_hostname = hostname;
}

void    User::setPassword(const std::string& password)
{
	this->_password = password;
}

void    User::setAuthenticated(bool authenticated)
{
	this->_authenticated = authenticated;
}

void    User::setOperator(bool _operator)
{
	this->_operator = _operator;
}

void    User::setAway(bool _operator)
{
	this->_away = _operator;
}

void    User::setPartialMessage(const std::string& partialMessage)
{
	this->_partialMessage = partialMessage;
}

void    User::setUserEPollEvent(const epoll_event& socket)
{
	this->_userEPollEvent = socket;
}

void	User::setAddress(const sockaddr_in& address)
{
	this->_address = address;
}

void	User::setUsedPassword(bool b)
{
	this->_usedPassword = b;
}

void	User::setJoinedChannelsCount(int a)
{
	this->_joinedChannelsCount = a;
}

bool	User::sendMessage(const std::string& input) const
{
	std::string message;

	message += input;

	if (input[input.size() - 2] != '\r' && input[input.size() - 1] != '\n')
		message += "\r\n";

	if (getUserEPollEvent().data.fd >= 0)
	{	
		if (send(getUserEPollEvent().data.fd, message.c_str(), message.size(), 0) < 0)
			return (std::cerr << "Couldn't send message to " << getNickname()
					<< " because: " << strerror(errno)
					<< std::endl, false);
	}
	return (true);
}

static void	addressToString(std::ostream& outputSteam, const sockaddr_in& address)
{
	outputSteam << "\tAddress: "<< address.sin_addr.s_addr
		<< "\n\tFamily: " << address.sin_family
		<< "\n\tPort: " << address.sin_port;
}

std::ostream&	operator<<(std::ostream& outputStream, const User& user)
{
	outputStream << "User Info:\n" 
		<< "=> Full Name: " << user.getFullname()
		<< "\n=> Partial Message: " << user.getPartialMessage()
		<< "\n=> Hostname: " << user.getHostname()
		<< "\n=> Nickname: " << user.getNickname()
		<< "\n=> Username: " << user.getUsername()
		<< "\n=> Password: " << user.getPassword()
		<< "\n=> Is Authenticated: " << user.isAuthenticated()
		<< "\n=> Is Operator: " << user.isOperator()
		<< "\n=> User fd: "
		<< user.getUserEPollEvent().data.fd
		<< "\n=> User Address: \n";
	addressToString(outputStream, user.getAddress());
	
	return (outputStream);
}
