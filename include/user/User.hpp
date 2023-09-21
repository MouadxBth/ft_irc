#pragma once

# include <iostream>
# include <string>
# include <sys/epoll.h>
# include <netinet/in.h>

class User
{
    private:
        std::string     _username;
        std::string     _fullname;
        std::string     _hostname;
        std::string     _nickname;
        std::string     _password;
	    std::string	    _partialMessage;
        std::string     _awayMessage;
        bool            _authenticated;
        bool            _operator;
        bool            _away;

	    struct epoll_event		    _userEPollEvent;
        sockaddr_in     _address;

        bool             _usedPassword;
        int              _joinedChannelsCount;

    public:

        User();
        ~User();
        User(const User& instance);
        User& operator=(const User& instance);

        const std::string&    getUsername() const;
        const std::string&    getNickname() const;
        const std::string&    getFullname() const;
        const std::string&    getHostname() const;
        const std::string&    getPassword() const;
        const std::string&    getPartialMessage() const;
        const std::string&    getAwayMessage() const;
        
        bool            isAuthenticated() const;
        bool            isOperator() const;
        bool            isAway() const;

        const epoll_event&          getUserEPollEvent() const;
        const sockaddr_in&     getAddress() const;

        bool      hasUsedPassword() const;

        int         getJoinedChannelsCount() const;

        void    setUsername(const std::string& username);
        void    setNickname(const std::string& nickname);
        void    setFullname(const std::string& fullname);
        void    setHostname(const std::string& hostname);
        void    setPassword(const std::string& password);
        void    setPartialMessage(const std::string& msgpartial);
        void    setAuthenticated(bool authenticated);
        void    setOperator(bool boolean);
        void    setAway(bool boolean);

        void    setUserEPollEvent(const epoll_event& epollEvent);
        void    setAddress(const sockaddr_in& address);

        void    setUsedPassword(bool b);

        void    setJoinedChannelsCount(int count);

        bool    sendMessage(const std::string &message) const;

};

std::ostream&	operator<<(std::ostream& outputStream, const User& user);
