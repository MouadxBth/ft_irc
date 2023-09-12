/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/19 11:13:56 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/12 10:33:48 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <Server.hpp>

//ERROR REPLIES
#define ERR_UNKNOWN_COMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_NEED_MORE_PARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOT_REGISTERED(source)						"451 " + source + " :You have not registered"
#define ERR_ALREADY_REGISTERED(source)					"462 " + source + " :You may not reregister"
#define ERR_PASSWD_MISMATCH(source)						"464 " + source + " :Password incorrect"
#define ERR_NO_NICKNAME_GIVEN(source)						"431 " + source + " :Nickname not given"
#define ERR_NICKNAME_INUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_TOO_MANY_CHANNELS(source, channel)			"405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_NOT_ON_CHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NO_SUCH_CHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_BAD_CHANNEL_KEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_NOSUCHNICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(source, channel)			"404 " + source + " " + channel + " :Cannot send to channel"

// NUMERIC REPLIES
#define RPL_WELCOME(source)						"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."

// COMMAND REPLIES
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args


class Command
{

private:
    std::string _name;
    std::string _description;
    size_t  _code;
    bool    _authRequired;

public:
    Command();
    virtual ~Command();
    Command(const Command& instance);
    Command& operator=(const Command& instance);

    Command(const std::string& name, const std::string& description, size_t code, bool authRequired);

    void    execute(User *user, Data &data);

    virtual void executeCommand(User *user, Data &data) = 0;

    const std::string&  getName() const;
    const std::string&  getDescription() const;
    size_t  getCode() const;
    bool  isAuthRequired() const;
    
    void    setName(const std::string& name);
    void    setDescription(const std::string& description);
    void    setCode(size_t code);
    void    setAuthRequired(bool auth);
};
