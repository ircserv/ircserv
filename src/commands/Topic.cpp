// Command: TOPIC
// Parameters: <channel> [<topic>]

#include "IRCCommand.hpp"
#include "Response.hpp"

namespace IRCCommand {
void topic(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  UserRepository &userRepo = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  User *user = userRepo.getUser(clientSocket);
  std::vector<std::string> param = msg->getParams();
  std::string nick = user->getNickname(), cmd = msg->getCommand();
  // pram이 없는 경우
  if (param.empty()) return user->send(ERR_NEEDMOREPARAMS(nick, cmd));

  Channel *channel = channelRepo.getChannel(param[0]);
  // 토픽의 대상이 되는 채널이 없는 경우
  if (!channel) return user->send(ERR_NOSUCHCHANNEL(nick, param[0]));

  std::string channelName = channel->getName();
  // 실행자가 해당 채널에 등록되지 않은 경우
  if (!user->isJoined(channelName))
    return user->send(ERR_NOTONCHANNEL(nick, channelName));

  // 토픽 조회 및 설정 분리
  std::string topic = channel->getTopic();
  if (param.size() == 1 && topic.empty())
    return user->send(RPL_NOTOPIC(nick, channelName));
  if (param.size() == 1 && !topic.empty()) {
    user->send(RPL_TOPIC(nick, channelName, topic));
    return user->send(
        RPL_TOPICWHOTIME(user->getUsername(), channelName, nick, "0:0"));
  }

  // 토픽 설정 -> 유저 권한 확인 필요 -> 모드 미설정 시 모든 사용자가 가능
  if (channel->isTopicMode() && !channel->isOperator(*user))
    return user->send(ERR_CHANOPRIVSNEEDED(nick, channelName));

  // TOPIC #chan : -> 토픽을 제거하라는 의미
  if (param[1].empty()) {
    channel->setTopic("");
    return channel->send(*user,
                         ":" + nick + " TOPIC " + channelName + " :" + topic);
  }

  channel->setTopic(param[1]);
  topic = channel->getTopic();
  channel->send(*user, ":" + nick + " TOPIC " + channelName + " :" + topic);
  user->send(RPL_TOPIC(nick, channelName, topic));
}
}  // namespace IRCCommand