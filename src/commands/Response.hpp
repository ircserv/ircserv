#ifndef RESPONSE_HPP
#define RESPONSE_HPP

// itlk
/* Welcome Message */
#define RPL_WELCOME(client) \
  (":NIRC 001 " + client + " :Welcome to the NOTHING Network")
#define RPL_YOURHOST(client) \
  (":NIRC 002 " + client + " :Your host is NIRC, running version v1.0.0")
#define RPL_CREATED(client, date) \
  (":NIRC 003 " + client + " :This server was created " + date)
#define RPL_MYINFO(client) (":NIRC 004 " + client + " :NIRC v1.0.0")
#define RPL_ISUPPORT(client) \
  (":NIRC 005 " + client + " : :are supported by this server")

#define RPL_UMODEIS(client, modes) ("221 " + client + " " + modes)

#define RPL_CHANNELMODEIS(client, channel, modes, args) \
  ("324 " + client + " " + channel + " " + modes + " " + args)
#define RPL_NOTOPIC(client, channel) \
  ("331 " + client + " " + channel + " :No topic is set")
#define RPL_TOPIC(client, channel, topic) \
  ("332 " + client + " " + channel + " :" + topic)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) \
  ("333 " + client + " " + channel + " " + nick + " " + setat)

#define RPL_INVITING(client, nick, channel) \
  ("341 " + client + " " + nick + " " + channel)

#define RPL_NAMREPLY(client, symbol, channel, users) \
  ("353 " + client + " " + symbol + " " + channel + " :" + users)
#define RPL_ENDOFNAMES(client, channel) \
  ("366 " + client + " " + channel + " :End of /NAMES list")

#define ERR_NOSUCHNICK(client, nick) \
  ("401 " + client + " " + nick + " :No such nick/channel")
#define ERR_CANNOTSENDTOCHAN(client, nick) \
  ("404 " + client + " " + nick + " :Cannot send to channel")
#define ERR_NOSUCHCHANNEL(client, channel) \
  ("403 " + client + " " + channel + " :No such channel")
#define ERR_TOOMANYCHANNELS(client, channel) \
  ("405 " + client + " " + channel + " :You have joined too many channels")
#define ERR_NOORIGIN(client) ("409 " + client + " :No origin specified")

#define ERR_UNKOWNCOMMAND(client, command) \
  ("421 " + client + " " + command + " :Unknown command")

#define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(client, nick) \
  ("432 " + client + " " + nick + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(client, nick) \
  ("433 " + client + " " + nick + " :Nickname is already in use")

#define ERR_USERNOTINCHANNEL(client, nick, channel) \
  ("441 " + client + " " + nick + " " + channel +   \
   " :They aren't on that channel")
#define ERR_NOTONCHANNEL(client, channel) \
  ("442 " + client + " " + channel + ":You're not on that channel")
#define ERR_USERONCHANNEL(client, nick, channel) \
  ("443 " + client + " " + nick + " " + channel + " :is already on channel")

#define ERR_NOTREGISTERED(client) \
  ("451 " + client + " :You have not registered")

#define ERR_NEEDMOREPARAMS(client, command) \
  ("461 " + client + " " + command + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(client) \
  ("462 " + client + " :You may not reregister")
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect")

#define ERR_CHANNELISFULL(client, channel) \
  ("471 " + client + " " + channel + " :Cannot join channel (+l)")
#define ERR_INVITEONLYCHAN(client, channel) \
  ("473 " + client + " " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(client, channel) \
  ("475 " + client + " " + channel + " :Cannot join channel (+k)")
#define ERR_BADCHANMASK(channel) ("476 " + channel + " :Bad Channel Mask")

#define ERR_CHANOPRIVSNEEDED(client, channel) \
  ("482 " + client + " " + channel + " :You're not channel operator")
#define ERR_UMODEUNKNOWNFLAG(client) ("501 " + client + " :Unknown MODE flag")
#define ERR_USERSDONTMATCH(client) \
  ("502 " + client + " :Cant change mode for other users")

#define ERR_INVALIDMODEPARAM(client, channel, mode, param) \
  ("696 " + client + " " + channel + " " + mode + " :Invalid mode parameter")

#endif
