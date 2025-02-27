#include "Channel.hpp"

#include "IRCServer.hpp"

Channel::Channel()
    : name(""),
      symbol("="),
      users(),
      chops(),
      key(""),
      capacity(256),
      topic(""),
      mode(0) {}

Channel::Channel(std::string name, User &user) {
  this->name = name;
  this->users[user.getSocket()] = &user;
  this->chops.insert(&user);
  this->mode = 0;
  this->key = "";
  this->capacity = -1;
  this->topic = "";
}

Channel::~Channel() {}

Channel::Channel(const Channel &other) {
  this->name = other.name;
  this->users = other.users;
  this->chops = other.chops;
  this->mode = other.mode;
  this->key = other.key;
  this->capacity = other.capacity;
  this->topic = other.topic;
}

Channel &Channel::operator=(const Channel &other) {
  if (this != &other) {
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

void Channel::join(User &user) {
  users[user.getSocket()] = &user;
  invitedUsers.erase(&user);
}

void Channel::part(User &user) {
  users.erase(user.getSocket());
  chops.erase(&user);
}

void Channel::send(User &user, std::string message) {
  for (std::map<int, User *>::iterator it = users.begin(); it != users.end();
       ++it) {
    if (it->first != user.getSocket()) {
      (it->second)->send(message);
    }
  }
  return;
}

void Channel::kick(User &user) {
  users.erase(user.getSocket());
  if (chops.find(&user) != chops.end()) {
    chops.erase(&user);
  }
  user.kicked(this);
}

void Channel::broadcast(std::string message) {
  for (std::map<int, User *>::iterator it = users.begin(); it != users.end();
       ++it) {
    it->second->send(message);
  }
}

void Channel::toOperators(User &sender, std::string const &message) {
  for (std::set<User *>::iterator it = chops.begin(); it != chops.end(); ++it) {
    if (*it != &sender) {
      (*it)->send(message);
    }
  }
}

User *Channel::getUser(std::string const &username) {
  for (std::map<int, User *>::iterator it = users.begin(); it != users.end();
       ++it) {
    if (it->second->getNickname() == username) {
      return it->second;
    }
  }
  return NULL;
}

std::vector<User *> Channel::getUsers() {
  std::vector<User *> userList;
  for (std::map<int, User *>::iterator it = users.begin(); it != users.end();
       ++it) {
    userList.push_back(it->second);
  }
  return userList;
}

const std::string &Channel::getName() { return name; }

const std::string &Channel::getSymbol() { return symbol; }

const std::string &Channel::getTopic() { return topic; }

const std::string Channel::getModeString() {
  std::string modeString = "+";
  if (mode & MODE_INVITE) {
    modeString += "i";
  }
  if (mode & MODE_TOPIC) {
    modeString += "t";
  }
  if (mode & MODE_KEY) {
    modeString += "k";
  }
  if (mode & MODE_LIMIT) {
    modeString += "l";
  }

  return modeString;
}

bool Channel::hasUser(User &user) {
  return users.find(user.getSocket()) != users.end();
}

bool Channel::hasUser(std::string const &userName) {
  for (std::map<int, User *>::iterator it = users.begin(); it != users.end();
       ++it) {
    if (it->second->getNickname() == userName) {
      return true;
    }
  }
  return false;
}

bool Channel::isFull() {
  if (mode & MODE_LIMIT) {
    return users.size() >= capacity;
  }
  return false;
}

bool Channel::isInviteOnly() { return mode & MODE_INVITE; }

bool Channel::isKeyProtected() { return mode & MODE_KEY; }

bool Channel::isTopicMode() { return mode & MODE_TOPIC; }

bool Channel::isOperator(User &user) {
  return chops.find(&user) != chops.end();
}

bool Channel::isLimit() { return mode & MODE_LIMIT; }

bool Channel::isInvited(User &user) {
  return invitedUsers.find(&user) != invitedUsers.end();
}

bool Channel::isEmpty() { return users.size() == 0; }

bool Channel::authenticate(std::string key) { return key == this->key; }

void Channel::setMode(char mode, bool flag, void *key = NULL) {
  std::map<char, char> modemap;
  modemap['i'] = MODE_INVITE;
  modemap['t'] = MODE_TOPIC;
  modemap['k'] = MODE_KEY;
  modemap['l'] = MODE_LIMIT;

  if (flag && mode != 'o') {
    this->mode |= modemap[mode];
  } else {
    this->mode &= ~modemap[mode];
  }

  if (mode == 'k' && flag) {
    this->key = std::string(static_cast<char *>(key));
  } else if (mode == 'k') {
    this->key = "";
  } else if (mode == 'l' && flag) {
    this->capacity = std::strtod(static_cast<char *>(key), NULL);
  } else if (mode == 'l') {
    this->capacity = 256;
  } else if (mode == 'o') {
    User *user = getUser(static_cast<char *>(key));
    if (!flag)
      chops.erase(user);
    else
      chops.insert(user);
  }
}

void Channel::setTopic(const std::string &topc) { topic = topc; }

void Channel::invite(User &user) { invitedUsers.insert(&user); }

void Channel::uninvite(User &user) { invitedUsers.erase(&user); }

std::set<User *> Channel::getInvitedUsers() { return invitedUsers; }
