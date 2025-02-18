#include "Parser.hpp"
#include <iostream>
// Parser 클래스 구현
Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser& other) {
    (void)other;
}

Parser& Parser::operator=(const Parser& other) {
    if (this != &other) {
        // Parser는 현재 상태를 가지지 않으므로 특별한 복사가 필요 없음
    }
    return *this;
}

Message Parser::parseMessage(const std::string& rawMessage) const {
    Message msg(rawMessage);
    
    if (rawMessage.empty()) {
        return msg;
    }

    parsePrefix(rawMessage, msg);
    parseCommand(rawMessage, msg);
    parseParameters(rawMessage, msg);
    
    return msg;
}

void Parser::parsePrefix(const std::string& rawMessage, Message& msg) const {
    if (rawMessage.empty() || rawMessage[0] != PREFIX_DELIMITER) {
        return;
    }
    
    size_t spacePos = rawMessage.find(' ');
    if (spacePos != std::string::npos) {
        // setter를 통해 설정
        msg.setPrefix(rawMessage.substr(1, spacePos - 1));
    }
}

void Parser::parseCommand(const std::string& rawMessage, Message& msg) const {
    size_t start = (rawMessage[0] == PREFIX_DELIMITER) ? 
        rawMessage.find(' ') + 1 : 0;
    
    if (start >= rawMessage.length()) {
        return;
    }

    size_t end = rawMessage.find(' ', start);
    if (end != std::string::npos) {
        msg.setCommand(rawMessage.substr(start, end - start));
    } else {
        msg.setCommand(rawMessage.substr(start));
    }
}

void Parser::parseParameters(const std::string& rawMessage, Message& msg) const {
    size_t start = rawMessage.find(msg.getCommand()) + msg.getCommand().length();
    std::vector<std::string> params;
    
    if (start >= rawMessage.length()) {
        return;
    }
    while(true){
        start = rawMessage.find_first_not_of(' ', start);
        if (start == std::string::npos){
            break;
        }
        size_t end = rawMessage.find(' ', start);
        if (end != std::string::npos) {
            params.push_back(rawMessage.substr(start, end - start));
        } else {
            params.push_back(rawMessage.substr(start));
            break;
        }
        start = end;
    }
    msg.setParams(params);
}
