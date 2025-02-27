#ifndef CHANNEL_REPOSITORY_HPP
#define CHANNEL_REPOSITORY_HPP

#include <map>
#include <string>

#include "Channel.hpp"

class Channel;

class ChannelRepository {
 public:
  static ChannelRepository &getInstance();
  static void destroy();

  void addChannel(Channel channel);
  void removeChannel(Channel &channel);
  Channel *getChannel(std::string const &name);
  bool hasChannel(std::string const &name);

 private:
  static ChannelRepository *instance;
  std::map<std::string, Channel> channels;

  // PREVENT COPYING
  ChannelRepository();
  ~ChannelRepository();
  ChannelRepository(const ChannelRepository &other);
  ChannelRepository &operator=(const ChannelRepository &other);
};

#endif  // CHANNEL_REPOSITORY_HPP/