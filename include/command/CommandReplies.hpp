/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 19:50:02 by mbouthai          #+#    #+#             */
/*   Updated: 2023/09/19 13:18:15 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//ERROR REPLIES
#define ERR_UNKNOWN_COMMAND(source, command)				"421 " + source + " " + command + " :Unknown command"
#define ERR_NEED_MORE_PARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters"
#define ERR_NOT_REGISTERED(source)						"451 " + source + " :You have not registered"
#define ERR_ALREADY_REGISTERED(source)					"462 " + source + " :You may not reregister"
#define ERR_PASSWD_MISMATCH(source)						"464 " + source + " :Password incorrect"
#define ERR_NO_NICKNAME_GIVEN						"431 :No nickname given"
#define ERR_ERRONEUS_NICKNAME(source)						"432 " + source + " :Erroneous nickname"
#define ERR_NICK_COLLISION(source, user, host)						"436 " + source + " :Nickname collision KILL from " + user + "@" + host + ""
#define ERR_UNAVAIL_RESOURCE(source)						"437 " + source + " :Nick/channel is temporarily unavailable"

#define ERR_NOSUCHNICK(nickname)						"401 :" + nickname + ":No such nick/channel"
#define ERR_TOOMANYTARGETS(target, errorCode, abortMsg) "" + target + " :" + errorCode + " recipients. " + abortMsg + ""
#define ERR_NORECIPIENT(source)						"411 :No recipient given ("+ source +")"
#define ERR_NOTEXTTOSEND						"412 :No text to send"
#define ERR_NOTOPLEVEL(source)						"411 :No recipient given ("+ source +")"
#define ERR_WILDTOPLEVEL(source)						"411 :No recipient given ("+ source +")"

#define ERR_RESTRICTED						"484 :Your connection is restricted!"
#define ERR_NICKNAME_INUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_NO_SUCH_CHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel"
#define ERR_NO_SUCH_NICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_USER_NOT_IN_CHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHAN_O_PRIVS_NEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator"
#define ERR_CANNOT_SEND_TO_CHAN(source, channel)			"404 " + source + " " + channel + " :Cannot send to channel"

#define ERR_NOT_ON_CHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel"
#define ERR_USERONCHANNEL(user, channel) "443 " + user + " " + channel + " :is already on channel"

#define ERR_TOO_MANY_CHANNELS(channel)			"405 " + channel + " :You have joined too many channels"

#define ERR_CHANNEL_IS_FULL(channel)				"471 " + channel + " :Cannot join channel (+l)"
#define ERR_INVITE_ONLY_CHAN(channel)				"473 " + channel + " :Cannot join channel (+i)"
#define ERR_BANNED_FROM_CHAN(channel)				"474 " + channel + " :Cannot join channel (+b)"
#define ERR_BAD_CHANNEL_KEY(channel)				"475 " + channel + " :Cannot join channel (+k)"


// NUMERIC REPLIES
#define RPL_WELCOME(nick, user, host)	"001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host
#define RPL_YOURHOST(nick, servername, version)   "002 " + nick + " :Your host is " + servername + ", running version " + version
#define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date
#define RPL_MYINFO(nick, servername, version, userModes, channelModes) "004 " + nick + " " + servername + " " + version + " " + userModes + " " + channelModes
#define RPL_AWAY(nick, message)	"301 " + nick + " :" + message

#define RPL_MOTDSTART(nick, server) "375 " + nick + " :- " + server + " Message of the day - "
#define RPL_MOTD(nick, text) "372 " + nick + " :- " + text
#define RPL_ENDOFMOTD(nick) "376 " + nick + " :End of MOTD command"


#define RPL_NAMREPLY(nick, channel, list) "353 " + nick + " = " + channel + " :" + list
#define RPL_ENDOFNAMES(nick, channel) "366 " + nick + " " + channel + " :End of /NAMES list"

#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + nick + " " + command + " :Unknown command"
#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"

#define ERR_NOMOTD(nick) "422 " + nick + " :MOTD File is missing"

#define DUMMY(nick) "000 " + nick + " :"



// ====
//#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users + ""
//#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."

#define RPL_NOTOPIC(channel)					"331 " + channel + " :No topic is set"
#define RPL_TOPIC(channel, topic)					"332 " + channel + " :" + topic

// COMMAND REPLIES
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args

#define RPL_INVITING(channel, nick)					"341 " + channel + " " + nick