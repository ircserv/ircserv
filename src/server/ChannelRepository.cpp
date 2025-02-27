#include "ChannelRepository.hpp"

ChannelRepository *ChannelRepository::instance = NULL;

ChannelRepository::ChannelRepository() {}

ChannelRepository &ChannelRepository::getInstance() {
  if (instance == NULL) {
    instance = new ChannelRepository();
  }
  return *instance;
}

void ChannelRepository::destroy() {
  if (instance != NULL) {
    instance->channels.clear();
    delete instance;
  }
}

void ChannelRepository::addChannel(Channel channel) {
  if (hasChannel(channel.getName())) {
    throw std::invalid_argument("Channel already exists");
  }
  channels.insert(std::make_pair(channel.getName(), Channel(channel)));
}

void ChannelRepository::removeChannel(Channel &channel) {
  if (!hasChannel(channel.getName())) {
    throw std::invalid_argument("Channel does not exist");
  }
  std::set<User *> invitedUsers = channel.getInvitedUsers();
  for (std::set<User *>::iterator it = invitedUsers.begin();
       it != invitedUsers.end(); ++it) {
    (*it)->unInvited(channel.getName());
  }
  channels.erase(channel.getName());
}

ChannelRepository::~ChannelRepository() {}

Channel *ChannelRepository::getChannel(std::string const &name) {
  if (!hasChannel(name)) {
    return NULL;
  }
  return &channels[name];
}

bool ChannelRepository::hasChannel(std::string const &name) {
  if (channels.find(name) != channels.end()) {
    return true;
  }
  return false;
}
