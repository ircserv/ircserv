#include "User.hpp"

User::User()
: client(), nickname(""), username(""), realname(""), channels()
{
}

User::User(int socket) 
  : client(socket),
    nickname(""),
    username(""),
    realname(""),
    channels()
  {}

User::~User(){}

User::User(const User & other)
: client(other.client), nickname(other.nickname), username(other.username), realname(other.realname), channels(other.channels){}

User & User::operator=(const User & other)
{
  if(this != &other)
  {
    this->client = other.client;
    this->nickname = other.nickname;
    this->username = other.username;
    this->realname = other.realname;
    this->channels = other.channels;
  }
  return *this;
}

int User::getSocket() const
{
  return client.getSocket();
}

std::string User::getNickname() const
{
  return nickname;
}

std::string User::getUsername() const
{
  return username;
}

std::string User::getRealname() const
{
  return realname;
}

void User::join(Channel & channel)
{
  channel.join(*this);
  channels[channel.getName()] = &channel;
}

void User::part(Channel & channel)
{
  channel.part(*this);
  channels.erase(channel.getName());
}

void User::send(std::string message)
{
  client.send(message.c_str());
  IRCServer &server = IRCServer::getInstance();
  server.enableWriteEvent(client.getSocket());
}

void User::sendBufferFlush()
{
  client.sendBufferFlush();
}

std::string User::receive()
{
  return client.receive();
}

void User::setNickname(std::string nickname)
{
  this->nickname = nickname;
}

void User::setUsername(std::string username)
{
  this->username = username;
}

void User::setRealname(std::string realname)
{
  this->realname = realname;
}

std::vector<Channel> User::getChannels()
{
  std::vector<Channel> chans;
  for(std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
  {
    chans.push_back(*(it->second));
  }
  return chans;
}

bool User::isJoined(Channel & channel)
{
  if(channels.find(channel.getName()) != channels.end()){
    return true;
  }
  return false;
}
