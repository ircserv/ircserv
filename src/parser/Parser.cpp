#include "Parser.hpp"

#include <sstream>

#include "../utils/utils.hpp"

// Parser 클래스 구현
Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser& other) { (void)other; }

Parser& Parser::operator=(const Parser& other) {
  (void)other;
  return *this;
}

Message Parser::parseMessage(const std::string& rawMessage) const {
  Message msg(rawMessage);

  if (rawMessage.empty()) {
    return msg;
  }

  std::string raw = rawMessage;
  parsePrefix(raw, msg);
  parseCommand(raw, msg);
  parseParameters(raw, msg);
  std::vector<std::string> params = msg.getParams();

  return msg;
}

void Parser::parsePrefix(std::string& rawMessage, Message& msg) const {
  if (rawMessage.empty() || rawMessage[0] != PREFIX_DELIMITER) {
    msg.setPrefix("");
    return;
  }

  rawMessage.erase(0, 1);
  size_t spacePos = rawMessage.find(' ');
  if (spacePos != std::string::npos) {
    // setter를 통해 설정
    msg.setPrefix(rawMessage.substr(0, spacePos - 1));
    rawMessage = rawMessage.substr(spacePos + 1);
    return;
  }

  msg.setPrefix(rawMessage.substr(1));
  rawMessage = "";
}

void Parser::parseCommand(std::string& rawMessage, Message& msg) const {
  std::string command;
  std::stringstream ss;
  if (rawMessage.empty()) {
    msg.setCommand("");
    return;
  }

  ss << rawMessage;
  ss >> command;
  msg.setCommand(command);
  rawMessage = rawMessage.substr(command.size());
  return;
}

void Parser::parseParameters(std::string& rawMessage, Message& msg) const {
  std::vector<std::string> result;
  std::string trailing = "";
  bool hasTrail = hasTrailing(rawMessage);
  if (rawMessage.empty()) {
    return msg.setParams(result);
  }
  if (hasTrail) {
    trailing = rawMessage.substr(rawMessage.find(" :") + 2);
    rawMessage = rawMessage.substr(0, rawMessage.find(" :"));
  }

  std::stringstream ss;
  std::string param;

  ss << rawMessage;
  while (!ss.eof()) {
    ss >> param;
    if (param.empty()) {
      break;
    }
    result.push_back(param);
    param = "";
  }

  if (hasTrail) {
    result.push_back(trailing);
  }
  msg.setParams(result);
  return;
}

bool Parser::hasTrailing(std::string const& raw) const {
  if (raw.empty()) return false;
  if (raw.find(" :") == std::string::npos) {
    return false;
  }
  return true;
}