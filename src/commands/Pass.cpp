/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:32:36 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:14 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: PASS
// Parameters: <password>

// 주어진 파라미터가 서버에 설정된 암호와 동일한지 확인한다.
// 성공 시 특별한 응답 없음

// ERR_NEEDMOREPARAMS (461) - 파라미터 부족인데, 우리 상황에선 불가능
// ERR_ALREADYREGISTERED (462) - 이미 등록되어 있는데, PASS를 다시 시도하는 경우
// -> 불가능~ ERR_PASSWDMISMATCH (464) - 패스워드 일치하지 않음! -> "<client>
// :Password incorrect" 464 "<client> :Password incorrect"

// PASS의 경우 461, 462, 464의 format을 충족하여 반환하려고 할 경우
// 소켓을 통해 client를 추적하는 과정이 필요함(client name을 구하는 과정)
// 그러나 일반적으로, PASS에 응답이 있는 경우 실패로 판단하기 때문에 적절한
// 메세지를 반환해도 된다.

#include <iostream>

#include "IRCCommand.hpp"

namespace IRCCommand {
void pass(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  IRCServer &server = IRCServer::getInstance();
  UserRepository &users = UserRepository::getInstance();
  User *user = users.getUser(clientSocket);
  std::vector<std::string> params = msg->getParams();
  std::string username =
      user->getNickname().empty() ? "unknown" : user->getNickname();

  if (params.size() == 0) {
    return user->send(ERR_NEEDMOREPARAMS(username, msg->getCommand()));
  }

  if (user->isauthentified()) {
    return user->send(ERR_ALREADYREGISTERED(username));
  }

  std::string password = params[0];
  if (!server.authenticate(password)) {
    return user->send(ERR_PASSWDMISMATCH(username));
  }
  user->setAuth(true);
  return;
}
}  // namespace IRCCommand