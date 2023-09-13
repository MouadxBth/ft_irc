#pragma once

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include "User.hpp"

class Channel
{
    private:
        std::string                         _name;
        std::string                         _topic;
        std::string                         _password;
        size_t                              _maximumCapacity;
        std::map<std::string, std::pair<User *, bool> >                               _users;
        bool                                _inviteOnly;
        bool                                _private;
        bool                                _topicOperator;

    public:
        Channel();
        ~Channel();
        Channel(const Channel& instance);
        Channel& operator=(const Channel& instance);

        const std::string&                    getName() const;
        const std::string&                    getTopic() const;
        const std::string&                    getPassword() const;
        
        size_t                          getMaximumCapacity() const;
        const std::map<std::string, std::pair<User *, bool> >&                    getUsers() const;
        
        bool                            isPrivate() const;
        bool                            isInviteOnly() const;
        bool                            isTopicOperator() const;

        void    setName(std::string& name);
        void    setTopic(std::string& topic);
        void    setPassword(std::string& topic);

        void    setMaximumCapacity(size_t capacity);
        
        void    setInviteOnly(bool inviteOnly);
	    void    setPrivate(bool _private);
        void    setTopicOperator(bool _private);


        const std::string getUserNicknames() const;
        
        const std::pair<User *, bool>& getUser(const std::string& nickname) const;

        void addUser(User *user, bool isOperator = false);
        void removeUser(const std::string& nickname);
        
        bool containsUser(const std::string& nickname) const;

        void promoteUser(const std::string& nickname);
        void demoteUser(const std::string& nickname);

        void broadcast(const std::string& nickname, std::string& message) const;

        void announce(std::string& message) const;
};

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel);
