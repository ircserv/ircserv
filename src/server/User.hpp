#ifndef USER_HPP
#define USER_HPP

#include <map>
#include <string>
#include <vector>

#include "../tcp/TCPClient.hpp"
#include "./IRCServer.hpp"
#include "Channel.hpp"

class Channel;

class User {
 private:
  TCPClient client;
  std::string nickname;
  std::string username;
  std::string hostname;
  std::string servername;
  std::string realname;
  std::set<std::string> invitedChannels;
  bool authenticated;
  std::map<std::string, Channel *> channels;
  bool registered;
  bool quitFlag;

 public:
  User();
  User(int socket);
  ~User();
  User(const User &other);
  User &operator=(const User &other);

  std::string getNickname() const;
  std::string getUsername() const;
  std::string getRealname() const;
  int getSocket() const;

  void join(Channel &channel);
  void part(Channel &channel);
  void send(std::string message);
  void kicked(Channel *channel);
  void broadcast(std::string msg);
  bool sendBufferFlush();
  std::vector<std::string> receive();

  void setNickname(std::string nickname);
  void setUsername(std::string username);
  void setRealname(std::string realname);
  void setHostname(const std::string hostname);
  void setServername(const std::string servername);

  const std::string getFullName();

  std::vector<Channel *> getChannels();
  bool isJoined(std::string const &channel);

  void setAuth(bool status);
  void setRegistered(bool status);
  bool isauthentified();  // pass PASS command
  bool isRegistered();    // pass NICK and USER command
  void quit();
  bool isQuit();

  void invited(std::string channelName);
  void unInvited(std::string channelName);
  std::set<std::string> getInvitedChannels();
};

#endif