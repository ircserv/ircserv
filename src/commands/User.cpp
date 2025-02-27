/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:52:43 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:23 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: USER
// Parameters: <username> 0 * <realname>
// USER <username> <hostname> <servername> <realname>

// 서버에서 해당 유저를 인식하기 위한 유저 정보를 설정한다. -> 따라서 최초
// 연결시만 사용된다. 실제 connect 시 NICK -> USER 순으로 전송, 동시에 보내지는
// 경우도 있으나 알바노

// NICK 을 기반으로 유저를 인식하여 해당 유저 정보를 저장한다.

// /conect 555 999 hi
// -> PASS 통과했다고 가정 -> USER hi 8 * Realname -> 이때 Realname은 사용자가
// 별도 설정하지 않는 이상
// -> 사용자 플래그 8 은 일반 유저를 의미, 대부분의 irc 프로토콜에서 공통적임
// -> <username>도 별도로 지정하지 않는 이상, 유저의 닉네임을 그대로 사용
// username 변수에 대한 길이는 USERLEN으로 정의되어야 한다. -> 최소 길이는 1

// ERR_NEEDMOREPARAMS (461) - username이 비어있는 경우
// "<client> <command> :Not enough parameters"
// ERR_ALREADYREGISTERED (462) - 이미 등록한 이후, 해당 명령어 재시도
// "<client> :You may not reregister"

#include "IRCCommand.hpp"
namespace IRCCommand {
void user(fd clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();

  UserRepository &users = UserRepository::getInstance();
  User *user = users.getUser(clientSocket);

  if (!user->isauthentified()) {
    return user->send(ERR_PASSWDMISMATCH(user->getNickname()));
  }

  if (!user->getRealname().empty()) {
    return user->send(ERR_ALREADYREGISTERED(user->getNickname()));
  }

  if (params.size() < 4) {
    return user->send(
        ERR_NEEDMOREPARAMS(user->getNickname(), msg->getCommand()));
  }

  user->setUsername(params[0]);
  user->setHostname(params[1]);
  user->setServername(params[2]);
  user->setRealname(params[3]);
}
}  // namespace IRCCommand