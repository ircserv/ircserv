#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include <string>
#include <sys/socket.h>
#include "../tcp/TCPServer.hpp"
#include "ChannelRepository.hpp"
#include "UserRepository.hpp"

class User;
class Channel;
class UserRepository;
class ChannelRepository;


class IRCServer {
    public:
      static IRCServer &getInstance();
      static void destroy();

      void start(int port);
      void on(const std::string event, EventCallback callback);
      void broadcast(const char* data);
      void send(fd clientSocket, const char* data);
      void disconnect(fd clientSocket);
      void disconnectAll();
      void enableWriteEvent(fd clientSocket);
      
    private:
      static IRCServer *instance;
      TCPServer server;
      std::map<std::string, EventCallback> events;
      UserRepository &users;
      ChannelRepository &channels;

      IRCServer();
      ~IRCServer();
      IRCServer(const IRCServer& other);
      IRCServer& operator=(const IRCServer& other);

    
    
    static void acceptCallback(fd eventSocket, void* server);
    static void disconnectCallback(fd eventSocket, void* server);
    static void readCallback(fd eventSocket, void* server);
    static void writeCallback(fd eventSocket, void* server);
    static void errorCallback(fd eventSocket, void* server);

};

#endif