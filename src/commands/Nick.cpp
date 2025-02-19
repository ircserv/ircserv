/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 14:50:04 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/20 03:01:47 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: NICK
// Parameters: <nickname>

// 서버 초기 연결 시 닉네임을 설정하거나 이전 닉네임을 변경한다.
// 이미 존재할 시, 에러 응답하고 해당 커맨드를 무시한다.

/* Valid Nick*/
// 서버는 별명에 최소한 모든 영숫자 문자, 대괄호와 중괄호( []{}), 백슬래시( \), 파이프( |) 문자를 허용한다.
// no leading # character or other character advertized in CHANTYPES
// no leading colon (:)
// no ASCII space
// 가 아닌 이상 닉네임으로 가능, 최대 길이는 몰?루, 서버마다 첫 글자가 숫자인 경우를 금지하곤 함

// ERR_NONICKNAMEGIVEN (431) - 커맨드에 파라미터가 없음
// 431 nick "<client> :No nickname given"
// ERR_ERRONEUSNICKNAME (432) - 유요하지 않은 닉네임
// 432 "<client> <nick> :Erroneus nickname
// ERR_NICKNAMEINUSE (433) - 이미 사용중 (네트워크에서)
// 433 "<client> <nick> :Nickname is already in use"
// ERR_NICKCOLLISION (436) - 이미 사용중 (채널에서) -> 우리랑 상관 없음, 네트워크 병합 시 발생

#include "IRCCommand.hpp"

bool validateNickname(std::string const &nickname);

namespace IRCCommand{
  void nick(fd clientSocket, void* message){
    IRCServer &ircServer = IRCServer::getInstance();
    UserRepository &users = UserRepository::getInstance();
    User *user = users.getUser(clientSocket);
    Message *msg = static_cast<Message*>(message);
    std::cout << "[EVENT] NICK" << std::endl;
    
    if(!user->isRegistered()){
      user->send("451 " + user->getNickname() + " :You have not registered");
      ircServer.enableWriteEvent(clientSocket);
      return ;
    }
    
    std::vector<std::string> params = msg->getParams();
    if (params.size() == 0) {
      user->send("431" + user->getNickname() + " :No nickname given");
      ircServer.enableWriteEvent(clientSocket);
      return ;
    }

    std::string nickname = params[0];
    if(!validateNickname(nickname)){
      user->send("432" + user->getNickname() + " " + nickname + " :Erroneus nickname");
      ircServer.enableWriteEvent(clientSocket);
    }
    
    user->setNickname(nickname);

    if(!user->getNickname().empty() && !user->getUsername().empty()){
      user->send(RPL_WELCOME(user->getNickname()));
      user->send(RPL_YOURHOST(user->getNickname()));
      user->send(RPL_CREATED(user->getNickname(), ircServer.getCreatedTime()));
      user->send(RPL_MYINFO(user->getNickname()));
      user->send(RPL_ISUPPORT(user->getNickname())); 
      ircServer.enableWriteEvent(clientSocket);  
    }
  }
}


bool validateNickname(std::string const &nickname){
  const std::string FORBIDDEN = " \t\n\r\f\v:#";
  if(nickname.empty())
    return false;
  if(nickname.find_first_of(FORBIDDEN) != std::string::npos){
    return false;
  }

  return true;
}