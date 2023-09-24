#pragma once

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <set>

# include "User.hpp"
# include "ChannelUserModes.hpp"

class Channel
{
    private:
        const std::string		_name;

        std::string		_topic;
        std::string		_password;
        size_t			_maximumCapacity;

        std::set<std::string>	_inviteList;
        std::set<std::string>	_banList;

        std::map<std::string, std::pair<User *, ChannelUserModes> >	_users;
        
        bool	_inviteOnly;
        bool	_topicOperator;
        bool	_key;
        bool	_userLimit;
        bool	_private;
        bool	_moderated;
        bool	_externalMessages;

    public:
        Channel();
        ~Channel();
        Channel(const Channel& instance);
        Channel& operator=(const Channel& instance);

		Channel(const std::string& name);

        const std::string&	getName() const;
        const std::string&	getTopic() const;
        const std::string&	getPassword() const;
        
        size_t	getMaximumCapacity() const;

        const std::set<std::string>&	getInviteList() const;
        const std::set<std::string>&	getBanList() const;

        const std::map<std::string, std::pair<User *, ChannelUserModes> >&	getUsers() const;

        const std::pair<User *, ChannelUserModes>&	getUser(const std::string& nickname) const;

		const std::string	getUserNicknames() const;

        const std::string	getChannelUserModes() const;
        
        bool	isInviteOnly() const;
        bool	isTopicSettableByChannelOperatorOnly() const;
        bool	isChannelKeySet() const;
        bool	isUserLimitSet() const;

        bool	isPrivate() const;
        bool	isModerated() const;
        bool	isExternalMessagesEnabled() const;

        void	setTopic(const std::string& topic);
        void	setPassword(const std::string& topic);
        void	setMaximumCapacity(size_t capacity);
        
        void	setInviteOnly(bool inviteOnly);        
        void	setTopicSettableByChannelOperatorOnly(bool _private);
	    void	setChannelKey(bool _private);
	    void	setUserLimit(bool _private);

	    void	setPrivate(bool _private);
	    void	setModerated(bool _private);
	    void	setExternalMessagesEnabled(bool _private);

        bool	isUserInvited(const std::string& nickname) const;
        
		bool	inviteUser(const std::string& nickname);	
        bool    unInviteUser(const std::string& nickname);
        
		bool	isUserBanned(const std::string& nickname) const;	

        bool	banUser(const std::string& nickname);
        bool	pardonUser(const std::string& nickname);
        
		bool	containsUser(const std::string& nickname) const;	

        void	addUser(User *user, ChannelUserModes& ChannelUserModes);
        void	removeUser(const std::string& nickname);

		bool	setOwner(const std::string& nickname, bool boolean);
	
        bool	setOperator(const std::string& nickname, bool boolean);

        bool	setVoice(const std::string& nickname, bool boolean);
	
        void	broadcast(const std::string& nickname, std::string& message) const;

        void	announce(const std::string& message) const;	
        void	announce(const std::vector<std::string>& messages) const;

};

std::ostream&	operator<<(std::ostream& outputStream, const Channel& channel);
