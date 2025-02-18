/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:32:36 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/18 19:10:31 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: PASS
// Parameters: <password>

// 주어진 파라미터가 서버에 설정된 암호와 동일한지 확인한다.
// 성공 시 특별한 응답 없음

// ERR_NEEDMOREPARAMS (461) - 파라미터 부족인데, 우리 상황에선 불가능
// ERR_ALREADYREGISTERED (462) - 이미 등록되어 있는데, PASS를 다시 시도하는 경우 -> 불가능~
// ERR_PASSWDMISMATCH (464) - 패스워드 일치하지 않음! -> "<client> :Password incorrect"
// 464 "<client> :Password incorrect"

// PASS의 경우 461, 462, 464의 format을 충족하여 반환하려고 할 경우
// 소켓을 통해 client를 추적하는 과정이 필요함(client name을 구하는 과정)
// 그러나 일반적으로, PASS에 응답이 있는 경우 실패로 판단하기 때문에 적절한 메세지를 반환해도 된다.

#include "IRCCommand.hpp"
#include <iostream>

namespace IRCCommand {
  void pass(int clientSocket, void* message){
    Message *msg = static_cast<Message*>(message);
    IRCServer &server = IRCServer::getInstance();
    UserRepository &users = UserRepository::getInstance();
    User *user = users.getUser(clientSocket);
    std::vector<std::string> params = msg->getParams();

    if(params.size() == 0){
      // TODO: ERRO_NEEDMOREPARAMS
      user->send("461 " + user->getNickname() + " " + msg->getCommand() + " :Not enough parameters");
      server.enableWriteEvent(clientSocket);
      return;
    }

    if (user->isRegistered()) {
      // TODO: ERR_ALREDYREGISTERED
      user->send("462 " + user->getNickname() + " :You may not reregister");
      server.enableWriteEvent(clientSocket);
      return;
    }
    std::string password = params[0];
    if(!server.authenticate(password)){
      // ERR_PASSWDMISMATCH
      user->send("464 " + user->getNickname() + " :Password incorrect");
      server.enableWriteEvent(clientSocket);
      return ; 
    }
    user->setRegistered(true);
    return ;
  }
}