#include "User.hpp"

User::User()
    : client(),
      nickname(""),
      username(""),
      hostname(""),
      servername(""),
      realname(""),
      authenticated(false),
      channels(),
      registered(false),
      quitFlag(false) {}

User::User(int socket)
    : client(socket),
      nickname(""),
      username(""),
      hostname(""),
      servername(""),
      realname(""),
      authenticated(false),
      channels(),
      registered(false),
      quitFlag(false) {}

User::~User() {}

User::User(const User &other)
    : client(other.client),
      nickname(other.nickname),
      username(other.username),
      hostname(other.hostname),
      servername(other.servername),
      realname(other.realname),
      authenticated(other.authenticated),
      channels(other.channels),
      registered(false),
      quitFlag(other.quitFlag) {}

User &User::operator=(const User &other) {
  if (this != &other) {
    this->client = other.client;
    this->nickname = other.nickname;
    this->username = other.username;
    this->hostname = other.hostname;
    this->servername = other.servername;
    this->realname = other.realname;
    this->authenticated = other.authenticated;
    this->channels = other.channels;
    this->registered = other.registered;
    this->quitFlag = other.quitFlag;
  }
  return *this;
}

int User::getSocket() const { return client.getSocket(); }

std::string User::getNickname() const { return nickname; }

std::string User::getUsername() const { return username; }

std::string User::getRealname() const { return realname; }

void User::join(Channel &channel) {
  channel.join(*this);
  invitedChannels.erase(channel.getName());
  channels[channel.getName()] = &channel;
}

void User::part(Channel &channel) {
  channel.part(*this);
  channels.erase(channel.getName());
}

void User::send(std::string message) {
  client.send(message.c_str());
  IRCServer &server = IRCServer::getInstance();
  server.enableWriteEvent(client.getSocket());
}

void User::kicked(Channel *channel) { channels.erase(channel->getName()); }

void User::broadcast(std::string msg) {
  std::vector<Channel *> channels = getChannels();
  for (std::vector<Channel *>::iterator it = channels.begin();
       it != channels.end(); ++it) {
    (*it)->send(*this, msg);
  }
}

bool User::sendBufferFlush() { return client.sendBufferFlush(); }

std::vector<std::string> User::receive() {
  std::vector<std::string> messages;
  while (true) {
    std::string message = client.receive();
    if (message.empty()) {
      break;
    }
    messages.push_back(message);
  }
  return messages;
}

void User::setNickname(std::string nickname) { this->nickname = nickname; }

void User::setUsername(std::string username) { this->username = username; }

void User::setHostname(const std::string hostname) {
  this->hostname = hostname;
}

void User::setServername(const std::string servername) {
  this->servername = servername;
}

const std::string User::getFullName() {
  return nickname + "!" + username + "@" + hostname;
}
void User::setRealname(std::string realname) { this->realname = realname; }

std::vector<Channel *> User::getChannels() {
  std::vector<Channel *> chans;
  for (std::map<std::string, Channel *>::iterator it = channels.begin();
       it != channels.end(); ++it) {
    chans.push_back(it->second);
  }
  return chans;
}

bool User::isJoined(std::string const &channelName) {
  if (channels.find(channelName) != channels.end()) {
    return true;
  }
  return false;
}

void User::setAuth(bool status) { this->authenticated = status; }

bool User::isauthentified() { return authenticated; }

void User::setRegistered(bool status) { this->registered = status; }

bool User::isRegistered() { return registered; }

void User::quit() { quitFlag = true; }

bool User::isQuit() { return quitFlag; }

void User::invited(std::string channelName) {
  invitedChannels.insert(channelName);
}

void User::unInvited(std::string channelName) {
  invitedChannels.erase(channelName);
}

std::set<std::string> User::getInvitedChannels() { return invitedChannels; }
