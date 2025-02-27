/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 02:58:39 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:07 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: JOIN
// Parameters: <channel>{,<juuchannel>} [<key>{,<key>}]
// Alt Params: 0

// 클라이언트가 해당 채널에 가입을 요청한다.
// 이때, 클라이언트는 해당 채널의 JOIN, PART, KICK, MSG 에 대한 모든 이벤트를
// 공유 받는다(가입 성공 시)

// 가입이 성공 시, 서버는 클라이언트로 해당 이벤트를 순서대로 보낸다.
// 1. JOIN의 성공 메세지
// 2. 채널의 토픽 or 332, 333
// 3. 채널에 가입된 사용자 목록 or 353, 366 -> 이때 해당 유저도 포함

// Key, 혹은 다양한 Chanel MODE에 따라서 JOIN은 실패할 수 있다.

// 서버는 클라이언트가 한번에 가입 시도하는 채널의 수를 제한 할 수 있다. -> 이는
// CHANLIMIT라는 매개변수로 정의되어야 한다.

// ERR_NEEDMOREPARAMS (461) - 파람 부족
// 461 "<client> <command> :Not enough parameters"

// ERR_NOSUCHCHANNEL (403) - 채널 없음
// 403 "<client> <channel> :No such channel"

// ERR_TOOMANYCHANNELS (405) - 가입 채널 최대 수 초과
// 405 "<client> <channel> :You have joined too many channels"

// ERR_BADCHANNELKEY (475) - 채널 가입에 필요한 키 틀림
// 475 "<client> <channel> :Cannot join channel (+k)"

// ERR_CHANNELISFULL (471) - 채널 꽉 참
// 471 "<client> <channel> :Cannot join channel (+l)"

// ERR_INVITEONLYCHAN (473) - 초대 안 됨
// 473 "<client> <channel> :Cannot join channel (+i)"

// ERR_BADCHANMASK (476) - 유효하지 않은 채널 이름
// 476 "<channel> :Bad Channel Mask"

// 해당 명령어의 경우, 정상 동작 시 전송되어야 하는 메세지가 정해져있고 반! 드!
// 시! 전송해야 하므로 별도의 정수 메세지로 정의되어 있음

// RPL_TOPIC (332) - 토픽
// 332 "<client> <channel> :<topic>"

// [ optionable ]RPL_TOPICWHOTIME (333) - 토픽 설정 시간
// 333 "<client> <channel> <nick> <setat>"
// RPL_NAMREPLY (353) - 사용자 목록
// 353 "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
// RPL_ENDOFNAMES (366) - 사용자 목록 끝남 알림
// 366 "<client> <channel> :End of /NAMES list"

// ABNF for channelname
// ChannelName := ( "#" | "&" ) chanstring
// chanstring := <! ( ' ' | ',' | '\a(BEL)') <any char>

#include "IRCCommand.hpp"

bool validateChannelName(const std::string &channelName);

namespace IRCCommand {
void join(fd clientSocket, void *message) {
  IRCServer &server = IRCServer::getInstance();
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();
  User *user = UserRepository::getInstance().getUser(clientSocket);

  if (params.empty()) {
    user->send(ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN"));
    server.enableWriteEvent(clientSocket);
    return;
  }

  ChannelRepository &channelRepo = ChannelRepository::getInstance();

  std::vector<std::string> channelNames = utils::split(params[0], ',');
  std::vector<std::string> keys = params.size() > 1
                                      ? utils::split(params[1], ',')
                                      : std::vector<std::string>();

  if (channelNames.size() == 1 && channelNames.front() == "0") {
    std::vector<Channel *> channels = user->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin();
         it != channels.end(); ++it) {
      (*it)->broadcast(":" + user->getFullName() + " PART " + (*it)->getName());
      user->part(**it);
      if ((*it)->isEmpty()) {
        channelRepo.removeChannel(**it);
      }
    }
    return;
  }

  size_t j = 0;
  for (std::vector<std::string>::iterator channelName = channelNames.begin();
       channelName != channelNames.end(); ++channelName) {
    if (!validateChannelName(*channelName)) {  // 채널이름이 유효하지 않은 경우
      user->send(ERR_BADCHANMASK(*channelName));
      continue;
    }

    Channel *channel =
        channelRepo.getChannel(*channelName);  // 채널이 존재하는지 확인

    if (!channel) {
      channelRepo.addChannel(Channel(*channelName, *user));
      channel = channelRepo.getChannel(*channelName);
    }
    // FIXME: 채널에 가입할 수 있는지 확인 추가 필요
    if (channel->isKeyProtected() && j >= keys.size()) {
      user->send(ERR_BADCHANNELKEY(user->getNickname(), *channelName));
      continue;
    }
    if (channel->isKeyProtected() && !channel->authenticate(keys[j++])) {
      user->send(ERR_BADCHANNELKEY(user->getNickname(), *channelName));
      continue;
    }
    if (channel->isFull()) {
      user->send(ERR_CHANNELISFULL(user->getNickname(), *channelName));
      continue;
    }
    if (channel->isInviteOnly()) {
      if (!channel->isInvited(*user)) {
        user->send(ERR_INVITEONLYCHAN(user->getNickname(), *channelName));
        continue;
      }
    }

    user->join(*channel);
    std::vector<User *> users = channel->getUsers();
    std::string userNames;
    for (std::vector<User *>::iterator it = users.begin(); it != users.end();
         ++it) {
      userNames += channel->isOperator(**it) ? "@" : "";
      userNames += (*it)->getNickname();
      userNames += *it == users.back() ? "" : " ";
    }
    channel->broadcast(":" + user->getFullName() + " JOIN " + *channelName);
    if (channel->getTopic().empty()) {
      user->send(
          RPL_TOPIC(user->getNickname(), *channelName, channel->getTopic()));
    } else {
      user->send(RPL_NOTOPIC(user->getNickname(), *channelName));
    }
    user->send(RPL_NAMREPLY(user->getNickname(), "=", *channelName, userNames));
    user->send(RPL_ENDOFNAMES(user->getNickname(), *channelName));
  }
}
}  // namespace IRCCommand

bool validateChannelName(const std::string &channelName) {
  if (channelName[0] != '#' && channelName[0] != '&') {
    return false;
  }

  std::string name = channelName.substr(1);
  if (name.empty()) {
    return false;
  }
  if (name.find_first_of(" ,\a") != std::string::npos) {
    return false;
  }

  return true;
}
