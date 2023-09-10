#pragma once

# include <iostream>
# include <string>
# include <poll.h>
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
        bool            _authenticated;
        bool            _operator;

	    pollfd		    _userSocket;
        sockaddr_in     _address;

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
        
        bool            isAuthenticated() const;
        bool            isOperator() const;

        const pollfd&          getUserSocket() const;
        const sockaddr_in&     getAddress() const;

        void    setUsername(const std::string& username);
        void    setNickname(const std::string& nickname);
        void    setFullname(const std::string& fullname);
        void    setHostname(const std::string& hostname);
        void    setPassword(const std::string& password);
        void    setPartialMessage(const std::string& msgpartial);
        void    setAuthenticated(bool authenticated);
        void    setOperator(bool boolean);

        void    setUserSocket(const pollfd& user_fd);
        void    setAddress(const sockaddr_in& address);
};

std::ostream&	operator<<(std::ostream& outputStream, const User& user);
