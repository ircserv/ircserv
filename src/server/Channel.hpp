#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include "User.hpp"
#include <set>

typedef std::string channelName;

class User;

class Channel {
  private:
    std::string name;
    std::map<int, User *> users;

  public:
    Channel();
    Channel(std::string name);
    ~Channel();
    Channel(const Channel & other);
    Channel & operator=(const Channel & other);
    void join(User &user);
    void part(User &user);
    void send(User &user, std::string message);
    void broadcast(std::string message);
    std::vector<User *> getUsers();
    std::string getName();
    
};


#endif