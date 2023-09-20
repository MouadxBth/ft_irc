#pragma once

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <set>

# include "User.hpp"

typedef struct s_modes
{
    bool voice;
    bool channelOperator;
}   Modes;

class Channel
{
    private:
        std::string                         _name;
        std::string                         _topic;
        std::string                         _password;
        size_t                              _maximumCapacity;
        std::map<std::string, std::pair<User *, Modes> >                               _users;
        
        std::set<std::string>            _inviteList;
        std::set<std::string>            _banList;
        
        bool                                _inviteOnly;
        bool                                _private;
        bool                                _topicOperator;
        bool                                _key;
        bool                                _userLimit;
        bool                                _moderated;
        bool                                _externalMessages;


    public:
        Channel();
        ~Channel();
        Channel(const Channel& instance);
        Channel& operator=(const Channel& instance);

        const std::string&                    getName() const;
        const std::string&                    getTopic() const;
        const std::string&                    getPassword() const;
        
        size_t                          getMaximumCapacity() const;
        const std::map<std::string, std::pair<User *, Modes> >&                    getUsers() const;
        
        const std::set<std::string>&                    getInviteList() const;
        const std::set<std::string>&                    getBanList() const;
        
        bool                            isPrivate() const; // 
        bool                            isInviteOnly() const;
        bool                            isTopicSettableByChannelOperatorOnly() const;
        bool                            isChannelKeySet() const;
        bool                            isUserLimitSet() const;
        bool                            isModerated() const;
        bool                            isExternalMessagesEnabled() const;


        void    setName(const std::string& name);
        void    setTopic(const std::string& topic);
        void    setPassword(const std::string& topic);

        void    setMaximumCapacity(size_t capacity);
        
        void    setInviteOnly(bool inviteOnly);
	    void    setPrivate(bool _private);
        void    setTopicSettableByChannelOperatorOnly(bool _private);
	    void    setChannelKey(bool _private);
	    void    setUserLimit(bool _private);
	    void    setModerated(bool _private);
	    void    setExternalMessagesEnabled(bool _private);


        const std::string getUserNicknames() const;
        
        const std::pair<User *, Modes>& getUser(const std::string& nickname) const;

        std::string getModes() const;

        void addUser(User *user, bool isOperator, bool hasVoice);
        void removeUser(const std::string& nickname);
        
        bool containsUser(const std::string& nickname) const;

        void promoteUser(const std::string& nickname);
        void demoteUser(const std::string& nickname);

        void grantVoice(const std::string& nickname);
        void revokeVoice(const std::string& nickname);

        void banUser(const std::string& nickname);
        void pardonUser(const std::string& nickname);
        
        bool isUserBanned(const std::string& nickname) const;

        void inviteUser(const std::string& nickname);

        bool isInvited(const std::string& nickname) const;

        void    removeInvite(const std::string& nickname);

        void broadcast(const std::string& nickname, std::string& message) const;

        void announce(std::string& message) const;

        void announce(std::vector<std::string>& messages) const;

};

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel);
