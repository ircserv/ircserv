/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 02:58:39 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/20 21:22:15 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: JOIN
// Parameters: <channel>{,<juuchannel>} [<key>{,<key>}]
// Alt Params: 0

// 클라이언트가 해당 채널에 가입을 요청한다.
// 이때, 클라이언트는 해당 채널의 JOIN, PART, KICK, MSG 에 대한 모든 이벤트를 공유 받는다(가입 성공 시)

// 가입이 성공 시, 서버는 클라이언트로 해당 이벤트를 순서대로 보낸다.
// 1. JOIN의 성공 메세지
// 2. 채널의 토픽 or 332, 333
// 3. 채널에 가입된 사용자 목록 or 353, 366 -> 이때 해당 유저도 포함

// Key, 혹은 다양한 Chanel MODE에 따라서 JOIN은 실패할 수 있다.

// 서버는 클라이언트가 한번에 가입 시도하는 채널의 수를 제한 할 수 있다. -> 이는 CHANLIMIT라는 매개변수로 정의되어야 한다.

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

// 해당 명령어의 경우, 정상 동작 시 전송되어야 하는 메세지가 정해져있고 반! 드! 시! 전송해야 하므로
// 별도의 정수 메세지로 정의되어 있음

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

// FIXME: split 함수를 추후 util namespace로 옮길 것
std::vector<std::string> split(std::string &str, char delim) {
  std::vector<std::string> result;
  std::string token;
  std::string temp = str;

  while(!temp.empty()) {
    size_t end = temp.find(delim);
    if(end == std::string::npos) {
      token = temp;
      result.push_back(token);
      break;
    }
    token = temp.substr(0, temp.find(delim));
    result.push_back(token);
    temp = temp.substr(temp.find(delim) + 1);
  }

  
  return result;
}


bool validateChannelName(const std::string &channelName){
  if(channelName[0] != '#' && channelName[0] != '&'){
    return false;
  }
  if(channelName.find_first_of(" ,\a") != std::string::npos){
    return false;
  }
  return true;
}


namespace IRCCommand {
  void join(fd clientSocket, void* message){
    IRCServer &server = IRCServer::getInstance();
    Message *msg = static_cast<Message*>(message);
    std::vector<std::string> params = msg->getParams();
    User *user = UserRepository::getInstance().getUser(clientSocket);

    std::cout << "[EVENT] JOIN" << std::endl;
    // TODO: PASS / NICK / USER 에 대한 예외처리
    if (!user->isauthentified()) {
      user->send("451 " + user->getNickname() + " :You have not registered");
      server.enableWriteEvent(clientSocket);
      return ;
    }

    if(params.size() < 1) {
      user->send("461" + msg->getCommand() + " :Not enough parameters");
      server.enableWriteEvent(clientSocket);
      return ;
    }

    ChannelRepository &channelRepo = ChannelRepository::getInstance();

    std::vector<std::string> channelNames = split(params[0], ',');
    std::vector<std::string> keys;
    if (params.size() > 1) {
      keys = split(params[1], ',');
    }
    for(size_t i = 0; i < channelNames.size(); i++){
      std::string channelName = channelNames[i];
      if(!validateChannelName(channelName)){ // 채널이름이 유효하지 않은 경우 
        user->send("476 " + channelName + " :Bad Channel Mask");
        continue;
      }
      Channel *channel = channelRepo.getChannel(channelName); // 채널이 존재하는지 확인
      
      if(channel){
        // FIXME: 채널에 가입할 수 있는지 확인 추가 필요
        // if(channel->isFull())
        // if(channel->isInviteOnly())
        // if(channel->isKeyProtected())
        // ban, except 는 요구 구현사항이 아니므로 생략
      } else { // 채널이 없으면 새로 생성
        channelRepo.addChannel(Channel(channelName, *user));
        channel = channelRepo.getChannel(channelName);
      }
      // SUCESS FLOW 
      // NAMREPLY
      user->join(*channel);
      std::vector<User *> users = channel->getUsers();
      std::string userNames;
      for(size_t i = 0; i < users.size(); i++){
        if(channel->isOperator(*users[i])){
          userNames += "@";
        }
        userNames += users[i]->getNickname();
        if(i != users.size() - 1){
          userNames += " ";
        }
      }
      channel->broadcast(":" + user->getNickname() + " JOIN " + channelName);
      if(!channel->getTopic().empty()) {
        user->send("332 "+ user->getNickname() + " " + channelName + " :" + channel->getTopic()); 
      }
      user->send(RPL_NAMREPLY(user->getNickname(), channel->getSymbol(), channelName, userNames));
      user->send("366 " + user->getNickname() + " " + channelName + " :End of /NAMES list");
    }
    server.enableWriteEvent(clientSocket);
  }
}