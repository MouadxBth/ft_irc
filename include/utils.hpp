#include <iostream>
#include <ctime>
#include "Server.hpp"

//ERROR REPLIES
#define ERR_UNKNOWN_COMMAND(source, command)				"421 " + source + " " + command + " :Unknown command\r\n"
#define ERR_NEED_MORE_PARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters\r\n"
#define ERR_NOT_REGISTERED(source)						"451 " + source + " :You have not registered\r\n"
#define ERR_ALREADY_REGISTERED(source)					"462 " + source + " :You may not reregister\r\n"
#define ERR_PASSWD_MISMATCH(source)						"464 " + source + " :Password incorrect\r\n"
#define ERR_NO_NICKNAME_GIVEN						"431 :No nickname given\r\n"
#define ERR_ERRONEUS_NICKNAME(source)						"432 " + source + " :Erroneous nickname\r\n"
#define ERR_NICK_COLLISION(source, user, host)						"436 " + source + " :Nickname collision KILL from " + user + "@" + host + "\r\n"
#define ERR_UNAVAIL_RESOURCE(source)						"437 " + source + " :Nick/channel is temporarily unavailable\r\n"
#define ERR_RESTRICTED						"484 :Your connection is restricted!\r\n"
#define ERR_NICKNAME_INUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use\r\n"
#define ERR_TOO_MANY_CHANNELS(source, channel)			"405 " + source + " " + channel + " :You have joined too many channels\r\n"
#define ERR_NOT_ON_CHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel\r\n"
#define ERR_NO_SUCH_CHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel\r\n"
#define ERR_BAD_CHANNEL_KEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)\r\n"
#define ERR_NO_SUCH_NICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel\r\n"
#define ERR_USER_NOT_IN_CHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel\r\n"
#define ERR_CHAN_O_PRIVS_NEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator\r\n"
#define ERR_CHANNEL_IS_FULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)\r\n"
#define ERR_CANNOT_SEND_TO_CHAN(source, channel)			"404 " + source + " " + channel + " :Cannot send to channel\r\n"

// NUMERIC REPLIES
#define RPL_WELCOME(source)						"001 " + source + " :Welcome " + source + " to the ft_irc network\r\n"
#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list.\r\n"

// COMMAND REPLIES
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args


std::string currentTimestamp(void);
std::string obtain_hostname(sockaddr_in user_addr);

bool isKnownCommand(std::string command);

std::vector<std::string> split(std::string& input, char delimiter);
std::string trim(const std::string& str);
