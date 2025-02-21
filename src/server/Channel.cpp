#include "Channel.hpp"
#include "IRCServer.hpp"


Channel::Channel()
: name(""), symbol("="), users(), chops(), key(""), capacity(0), topic(""), mode(0)
{

}


Channel::Channel(std::string name, User &user)
{
  this->name = name;
  this->users[user.getSocket()] = &user;
  this->chops.insert(&user);
  this->mode = 0;
  this->key = "";
  this->capacity = -1;
  this->topic = "";
}

Channel::~Channel()
{
}

Channel::Channel(const Channel & other)
{
  this->name = other.name;
  this->users = other.users;
  this->chops = other.chops;
  this->mode = other.mode;
  this->key = other.key;
  this->capacity = other.capacity;
  this->topic = other.topic;
}

Channel & Channel::operator=(const Channel & other)
{
  if(this != &other)
  {
    this->name = other.name;
    this->users = other.users;
    this->chops = other.chops;
    this->mode = other.mode;
    this->key = other.key;
    this->capacity = other.capacity;
    this->topic = other.topic;
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
  for(std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it){
    if (it->first != user.getSocket()) {
      (it->second)->send(message);
    }
  }
  return ;
}

void Channel::kick(User & user){
  users.erase(user.getSocket());
  if(chops.find(user.getNickname()) != chops.end()) {
    chops.erase(user.getNickname());
  }
  user.kicked(this);
}

void Channel::broadcast(std::string message)
{
  for(std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it){
    it->second->send(message);
  }
}

void Channel::toOperators(User &sender, std::string const &message){
  for (std::set<User *>::iterator it = chops.begin(); it != chops.end(); ++it){
    if(*it != &sender){
      (*it)->send(message);
    }
  }
}


std::vector<User *> Channel::getUsers()
{
    std::vector<User*> userList;
    for(std::map<int, User *>::iterator it = users.begin(); it != users.end(); ++it) {
        userList.push_back(it->second);
    }
    return userList;
}


const std::string &Channel::getName()
{
  return name;
}

const std::string &Channel::getSymbol()
{
  return symbol;
}

const std::string &Channel::getTopic()
{
  return topic;
}

bool Channel::hasUser(User & user)
{
  return users.find(user.getSocket()) != users.end();
}

bool Channel::isFull()
{
  if(mode & MODE_LIMIT) {
    return users.size() >= capacity;
  }
  return false;
}

bool Channel::isInviteOnly()
{
  return mode & MODE_INVITE;
}

bool Channel::isKeyProtected()
{
  return mode & MODE_KEY;
}

bool Channel::isOperator(User & user) {
  return chops.find(&user) != chops.end();
}

bool Channel::isLimit()
{
  return mode & MODE_LIMIT;
}

bool Channel::authenticate(std::string key)
{
  return key == this->key;
}

void Channel::setMode(std::string mode, std::vector<std::string> keys)
{
  (void)mode;
  (void)keys;
}


