#ifndef	RESPONSE_HPP
 #define	RESPONSE_HPP

/* Welcome Message */
#define RPL_WELCOME(client)										(":NIRC 001 " + client + " :Welcome to the NOTHING Network \r\n")
#define RPL_YOURHOST(client)									(":NIRC 002 " + client + " :Your host is PIRC, running version v1.0.0 \r\n")
#define RPL_CREATED(client, date)							(":NIRC 003 " + client + " :This server was created " + date + " \r\n")
#define	RPL_MYINFO(client)										(":NIRC 004 " + client + " :NIRC v1.0.0 \r\n")
// option은 최대 매개변수, 최대 토큰 수, 최대 채널입장 수, 최대 닉네임 길이 등등
#define RPL_ISUPPORT(client)									(":TIRC 005 " + client + " :option :are supported by this server \r\n")

#define ERR_NEEDMOREPARAMS(client, command)		("461 " + client + " " + command + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(client)					("462" + client + " :You may not reregister")

#endif