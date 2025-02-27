#ifndef IRC_COMMAND_HPP
#define IRC_COMMAND_HPP

#include <string>

#include "../parser/Message.hpp"
#include "../server/IRCServer.hpp"
#include "../server/User.hpp"
#include "../server/UserRepository.hpp"
#include "../utils/utils.hpp"
#include "Response.hpp"

#define CMD_CAP "CAP"
#define CMD_JOIN "JOIN"
#define CMD_NICK "NICK"
#define CMD_PART "PART"
#define CMD_PASS "PASS"
#define CMD_PING "PING"
#define CMD_PONG "PONG"
#define CMD_USER "USER"
#define CMD_QUIT "QUIT"
#define CMD_PRIVMSG "PRIVMSG"
#define CMD_KICK "KICK"
#define CMD_TOPIC "TOPIC"
#define CMD_INVITE "INVITE"
#define CMD_MODE "MODE"
#define CMD_NAMES "NAMES"

namespace IRCCommand {
// cap
void cap(int clientSocket, void* message);

// pass
void pass(int clientSocket, void* message);

// // nick
void nick(int clientSocket, void* message);

// // user
void user(int clientSocket, void* message);

// // join
void join(int clientSocket, void* message);

// PING
void ping(int clientSocket, void* message);

// // part
void part(int clientSocket, void* message);

// // quit
void quit(int clientSocket, void* message);

// // privmsg
void privmsg(int clientSocket, void* message);

// // notice
// void notice(int clientSocket, void* message);

// // for chop
void kick(int clientSocket, void* message);
void invite(int clientSocket, void* message);
void topic(int clientSocket, void* message);
void mode(int clientSocket, void* message);

void names(int clientSocket, void* message);

}  // namespace IRCCommand

#endif