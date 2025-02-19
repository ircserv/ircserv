#include "User.hpp"

User::User()
: client(), 
  nickname(""),
  username(""), 
  hostname(""),
  servername(""),
  realname(""), 
  registered(false),
  channels()
{
}

User::User(int socket) 
  :   
      client(socket),
      nickname(""),
      username(""), 
      hostname(""),
      servername(""),
      realname(""), 
      registered(false),
      channels()
  {}

User::~User(){}

User::User(const User & other)
: client(other.client), 
  nickname(other.nickname), 
  username(other.username), 
  hostname(other.hostname),
  servername(other.servername),
  realname(other.realname), 
  registered(other.registered),
  channels(other.channels)
{}

User & User::operator=(const User & other)
{
  if(this != &other)
  {
    this->client = other.client;
    this->nickname = other.nickname;
    this->username = other.username;
    this->hostname = other.hostname;
    this->servername = other.servername;
    this->realname = other.realname;
    this->registered = other.registered;
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
  if (nickname.empty()) {
    return "unknown";
  }
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

void User::sendBufferFlush() {
  client.sendBufferFlush();
}

std::vector<std::string> User::receive()
{
  std::vector<std::string> messages;
  while(true){
    std::string message = client.receive();
    if(message.empty()){
      break;
    }
    messages.push_back(message);
  }
  return messages;
}

void User::setNickname(std::string nickname)
{
  this->nickname = nickname;
}

void User::setUsername(std::string username)
{
  this->username = username;
}


void User::setHostname(const std::string hostname){
  this->hostname = hostname;
}
void User::setServername(const std::string servername){
  this->servername = servername;
}
void User::setRealname(std::string realname)
{
  this->realname = realname;
}

std::vector<Channel *> User::getChannels()
{
  std::vector<Channel *> chans;
  for(std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
  {
    chans.push_back(it->second);
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

void User::setRegistered(bool registered){
  this->registered = registered;
}

bool User::isRegistered(){
  return registered;
}
