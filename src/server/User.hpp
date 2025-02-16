#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <map>
#include "Channel.hpp"
#include <vector>
#include <iostream>
#include "../tcp/TCPClient.hpp"
#include "./IRCServer.hpp"

class Channel;

class User {
  private:
    TCPClient client;
    std::string nickname;
    std::string username;
    std::string realname;
    std::map<std::string, Channel *> channels;
  
  public:
    User();
    User(int socket);
    ~User();
    User(const User & other);
    User & operator=(const User & other);

    std::string getNickname() const;
    std::string getUsername() const;
    std::string getRealname() const;
    int getSocket() const;

    void join(Channel & channel);
    void part(Channel & channel);
    void send(std::string message);
    void sendBufferFlush();
    std::string receive();
    void broadcast(std::string message);

    void setNickname(std::string nickname);
    void setUsername(std::string username);
    void setRealname(std::string realname);

    std::vector<Channel> getChannels();
    bool isJoined(Channel & channel);
};  


#endif