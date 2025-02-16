#include "Channel.hpp"
#include "IRCServer.hpp"

Channel::Channel()
: name(""){}

Channel::Channel(std::string name)
{
  this->name = name;
}

Channel::~Channel()
{
}

Channel::Channel(const Channel & other)
{
  this->name = other.name;
  this->users = other.users;
}

Channel & Channel::operator=(const Channel & other)
{
  if(this != &other)
  {
    this->name = other.name;
    this->users = other.users;
  }
  return *this;
}

void Channel::join(User & user)
{
  users[user.getSocket()] = &user;
}

void Channel::part(User & user)
{
  users.erase(user.getSocket());
}

void Channel::send(User & user, std::string message)
{
  (void)user;
  (void)message;

  // TODO: Implement
  return ;
}

void Channel::broadcast(std::string message)
{
  // IRCServer *server = IRCServer::getInstance();
  for(std::map<int, User*>::iterator it = users.begin(); it != users.end(); ++it) {
          it->second->send(message);
  }
}


std::vector<User *> Channel::getUsers()
{
    std::vector<User*> userList;
    for(std::map<int, User*>::iterator it = users.begin(); it != users.end(); ++it) {
        userList.push_back(it->second);
    }
    return userList;
}


std::string Channel::getName()
{
  return name;
}
