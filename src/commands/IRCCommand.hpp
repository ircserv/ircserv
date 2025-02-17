#ifndef IRC_COMMAND_HPP
# define IRC_COMMAND_HPP

#include <string>
#include "../parser/Message.hpp"
#include "../server/User.hpp"
#include "../server/UserRepository.hpp"
#include "../server/IRCServer.hpp"

namespace IRCCommand{
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

  // // part
  // void part(int clientSocket, void* message);

  // // quit
  // void quit(int clientSocket, void* message);
  
  // // privmsg
  // void privmsg(int clientSocket, void* message);

  // // notice
  // void notice(int clientSocket, void* message);


  // // for chop
  // void kick(int clientSocket, void* message);
  // void invite(int clientSocket, void* message);
  // void topic(int clientSocket, void* message);
  // void mode(int clientSocket, void* message);
  
}

#endif