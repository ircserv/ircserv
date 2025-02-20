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
      void start(int port);
      void on(const std::string event, IRCEventCallback callback);
      void broadcast(const char* data);
      void send(fd clientSocket, const char* data);
      void disconnect(fd clientSocket);
      void disconnectAll();
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
    static void errorCallback(fd eventSocket);

};

#endif