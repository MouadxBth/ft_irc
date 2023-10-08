#pragma once

//ERROR REPLIES
#define RPL_WELCOME(nick, user, host)						"001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host

#define ERR_NOSUCHNICK(nick)								"401 " + nick + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(nick, channel)					"403 " + nick + " " + channel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel)					"404 " + nick + " " + channel + " :Cannot send to channel"
#define ERR_TOOMANYCHANNELS(nick, channel)					"405 " + nick + " " + channel + " :You have joined too many channels"

#define ERR_NORECIPIENT(nick, command)						"411 " + nick + ":No recipient given ("+ command +")"
#define ERR_NOTEXTTOSEND(nick)								"412 " + nick + ":No text to send"

#define ERR_UNKNOWNCOMMAND(nick, command)					"421 " + nick + " " + command + " :Unknown command"
#define ERR_NOMOTD(nick)									"422 " + nick + " :MOTD File is missing"


#define ERR_NONICKNAMEGIVEN(nick)							"431 " + nick + ":No nickname given"
#define ERR_ERRONEUSNICKNAME(nick, badNickname)				"432 " + nick + " " + badNickname + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick, givenNickname)				"433 " + nick + " " + givenNickname  + " :Nickname is already in use"
#define ERR_UNAVAILRESOURCE(nick, givenNickname)			"437 " + nick + " " + givenNickname + " :Nick/channel is temporarily unavailable"

#define ERR_USERNOTINCHANNEL(nick, target, channel)			"441 " + nick + " " + target + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(nick, channel)						"442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_USERONCHANNEL(user, channel)					"443 " + user + " " + channel + " :is already on channel"

#define ERR_NOTREGISTERED(nick)								"451 " + nick + " :You have not registered"

#define ERR_NEEDMOREPARAMS(nick, command)					"461 " + nick + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(nick)							"462 " + nick + " :You may not reregister"
#define ERR_PASSWDMISMATCH(nick)							"464 " + nick + " :Password incorrect"

#define ERR_KEYSET(nick, channel)							"464 " + nick + " " + channel + " :Channel key already set"

#define ERR_TOOMANYTARGETS(nick, target, abortMsg) 			"407 " + nick + " " + target + " :Too many targets. " + abortMsg

#define ERR_CHANNELISFULL(nick, channel)					"471 " + nick + " " + channel + " :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE(nick, character, channel)			"472 " + nick + " " + character + " :is unknown mode char to me for " + channel
#define ERR_INVITEONLYCHAN(nick, channel)					"473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN(nick, channel)					"474 " + nick + " " + channel + " :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(nick, channel)					"475 " + nick + " " + channel + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(nick, channel)					"482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_RESTRICTED(nick)								"484 " + nick + " :Your connection is restricted!"

// NUMERIC REPLIES
#define RPL_WELCOME(nick, user, host)									"001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host
#define RPL_YOURHOST(nick, servername, version)							"002 " + nick + " :Your host is " + servername + ", running version " + version
#define RPL_CREATED(nick, date)											"003 " + nick + " :This server was created " + date
#define RPL_MYINFO(nick, servername, version, userModes, channelModes)	"004 " + nick + " " + servername + " " + version + " " + userModes + " " + channelModes

#define RPL_AWAY(nick, message)											"301 " + nick + " :" + message

#define RPL_CHANNELMODEIS(nick, channel, message)								"324 " + nick + " " + channel + " " + message

#define RPL_NOTOPIC(nick, channel)										"331 " + nick + " " + channel + " :No topic is set"
#define RPL_TOPIC(nick, channel, topic)									"332 " + nick + " " + channel + " :" + topic

#define RPL_INVITING(nick, target, channel)								"341 " + nick + " " + target + " " + channel

#define RPL_NAMREPLY(nick, channel, list)								"353 " + nick + " = " + channel + " :" + list
#define RPL_ENDOFNAMES(nick, channel)									"366 " + nick + " " + channel + " :End of /NAMES list"

#define RPL_MOTDSTART(nick, server)										"375 " + nick + " :- " + server + " Message of the day - "
#define RPL_MOTD(nick, text)											"372 " + nick + " :- " + text
#define RPL_ENDOFMOTD(nick)												"376 " + nick + " :End of MOTD command"


#define DUMMY(nick) "000 " + nick + " :"