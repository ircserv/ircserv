/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:32:10 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/26 15:13:48 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "IRCServer.hpp"
#include "commands/IRCCommand.hpp"
#include <iostream>

int main(int argc, char *argv[]){

  if(!(argc == 3 || argc == 4)) {
    std::cerr << "Usage: " << argv[0] << "[<ip>] <server_port> <server_password>" << std::endl;
    return 1;
  }
  int port;
  std::string addr;
  std::string password;
  if(argc == 4){
    addr = std::string(argv[1]);
    if (!utils::isNumber(argv[2])) {
      std::cerr << "Port must be a number" << std::endl;
      return 1;
    }
    port = static_cast<int>(std::strtod(argv[2], NULL));
    password = std::string(argv[3]);
  } else {
    if (!utils::isNumber(argv[1])) {
      std::cerr << "Port must be a number" << std::endl;
      return 1;
    }
    port = static_cast<int>(std::strtod(argv[1], NULL));
    password = std::string(argv[2]);
  }

  
  IRCServer &server = IRCServer::getInstance();
  if(argc == 4) {
    server.setIp(addr);
  }
  server.setPort(port);
  server.setPassword(password);

  server.on(CMD_CAP, IRCCommand::cap);
  server.on(CMD_JOIN, IRCCommand::join);
  server.on(CMD_PASS, IRCCommand::pass);
  server.on(CMD_NICK, IRCCommand::nick);
  server.on(CMD_USER, IRCCommand::user);
  server.on(CMD_PING, IRCCommand::ping);
  server.on(CMD_PRIVMSG, IRCCommand::privmsg);
  server.on(CMD_KICK, IRCCommand::kick);
  server.on(CMD_QUIT, IRCCommand::quit);
  server.on(CMD_PART, IRCCommand::part);
  server.on(CMD_MODE, IRCCommand::mode);
  server.on(CMD_INVITE, IRCCommand::invite);
  server.on(CMD_TOPIC, IRCCommand::topic);
  server.on(CMD_NAMES, IRCCommand::names);
  
  try {
    server.start();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
