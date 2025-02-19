#ifndef	RESPONSE_HPP
 #define	RESPONSE_HPP

// itlk
/* Welcome Message */
#define	RPL_WELCOME(client)															(":NIRC 001 " + client + " :Welcome to the NOTHING Network \r\n")
#define	RPL_YOURHOST(client)														(":NIRC 002 " + client + " :Your host is NIRC, running version v1.0.0 \r\n")
#define	RPL_CREATED(client, date)												(":NIRC 003 " + client + " :This server was created " + date + " \r\n")
#define	RPL_MYINFO(client)															(":NIRC 004 " + client + " :NIRC v1.0.0 \r\n")
#define	RPL_ISUPPORT(client)														(":NIRC 005 " + client + " : :are supported by this server \r\n")

#define	RPL_UMODEIS(client, modes)											("221 " + client + " " + modes + " \r\n")

#define RPL_CHANNELMODEIS(client, channel, modes, args)	("324 " + client + " " + channel + " " + modes + " " + args + " \r\n")
#define	RPL_TOPIC(client, channel, topic)								("332 " + client + " " + channel + " :" + topic + " \r\n")

#define	RPL_INVITING(client, nick, channel)							("341 " + client + " " + nick + " " + channel + " \r\n")

#define	RPL_NAMREPLY(client, channel, users)						("353 " + client + " " + channel + " = " + users + " \r\n")
#define	RPL_ENDOFNAMES(client, channel)									("366 " + client + " " + channel + " :End of /NAMES list \r\n")

#define	ERR_NOSUCHNICK(client, nick)										("401 " + client + " " + nick + " :No such nick/channel \r\n")
#define	ERR_NOSUCHCHANNEL(client, channel)							("403 " + client + " " + channel + " :No such channel \r\n")
#define	ERR_TOOMANYCHANNELS(client, channel)						("405 " + client + " " + channel + " :You have joined too many channels \r\n")
#define	ERR_NOORIGIN(client)														("409 " + client + " :No origin specified \r\n")

#define	ERR_NONICKNAMEGIVEN(client)											("431 " + client + " :No nickname given \r\n")
#define	ERR_ERRONEUSNICKNAME(client, nick)							("432 " + client + " " + nick + " :Erroneus nickname \r\n")
#define	ERR_NICKNAMEINUSE(client, nick)									("433 " + client + " " + nick + " :Nickname is already in use \r\n")

#define	ERR_USERNOTINCHANNEL(client, nick, channel)			("441 " + client + " " + nick + " " + channel + " :They aren't on that channel \r\n")
#define	ERR_NOTONCHANNEL(client, channel)								("442 " + client + " " + channel + ":You're not on that channel \r\n")
#define	ERR_USERONCHANNEL(client, nick, channel)				("443 " + client + " " + nick + " " + channel + " :is already on channel \r\n")

#define	ERR_NEEDMOREPARAMS(client, command)							("461 " + client + " " + command + " :Not enough parameters \r\n")
#define	ERR_ALREADYREGISTERED(client)										("462 " + client + " :You may not reregister \r\n")
#define	ERR_PASSWDMISMATCH(client)											("464 " + client + " :Password incorrec \r\n")

#define	ERR_CHANNELISFULL(client, channel)							("471 " + client + " " + channel + " :Cannot join channel (+l) \r\n")
#define	ERR_INVITEONLYCHAN(client, channel)							("473 " + client + " " + channel + " :Cannot join channel (+i) \r\n")
#define	ERR_BADCHANNELKEY(client, channel)							("475 " + client + " " + channel + " :Cannot join channel (+k) \r\n")
#define	ERR_BADCHANMASK(channel)												("476 " + channel + " :Bad Channel Mask \r\n")

#define	ERR_CHANOPRIVSNEEDED(client, channel)						("482 " + client + " " + channel + " :You're not channel operator \r\n")
#define ERR_UMODEUNKNOWNFLAG(client)										("501 " + client + " :Unknown MODE flag \r\n")
#define ERR_USERSDONTMATCH(client)											("502 " + client + " :Cant change mode for other users \r\n")

#endif
