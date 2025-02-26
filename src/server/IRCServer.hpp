#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include <string>
#include <sys/socket.h>
#include "../tcp/TCPServer.hpp"
#include "ChannelRepository.hpp"
#include "UserRepository.hpp"
#include "../parser/Message.hpp"
#include "../parser/Parser.hpp"
#include "middleware/middleware.hpp"
#include "commands/Response.hpp"

class User;
class Channel;
class UserRepository;
class ChannelRepository;

typedef void (*IRCEventCallback)(fd clientSocket, void* message);
class IRCServer {
    public:
      static IRCServer &getInstance();
      static void destroy();

      std::string const &getCreatedTime() const;
      void setPort(int port);
      void setIp(std::string ip);
      void setPassword(std::string password);
      void start();
      void on(const std::string event, IRCEventCallback callback);
      void broadcast(const std::string &data);
      void send(fd clientSocket, const std::string &data);
      void disconnect(fd clientSocket);
      void enableWriteEvent(fd clientSocket);

      bool authenticate(std::string password);
      
    private:
      static IRCServer *instance;
      TCPServer server;
      std::map<std::string, IRCEventCallback> events;
      std::string password;
      std::string createdTime;

      IRCServer();
      ~IRCServer();
      IRCServer(const IRCServer& other);
      IRCServer& operator=(const IRCServer& other);

    
    
    static void acceptCallback(fd eventSocket);
    static void disconnectCallback(fd eventSocket);
    static void readCallback(fd eventSocket);
    static void writeCallback(fd eventSocket);

};

#endif