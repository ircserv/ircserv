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
    std::string copied_raw = rawMessage.substr(start);
    size_t trailing_pos = copied_raw.find(" :");
    std::string trailing = "";
    std::vector<std::string> result;

    if (trailing_pos != std::string::npos) {
        trailing = copied_raw.substr(trailing_pos + 2);
        copied_raw = copied_raw.substr(0, trailing_pos);
    }
    while(!copied_raw.empty()){
        start = copied_raw.find_first_not_of(' ', 0);
        size_t end = copied_raw.find(' ', start);
        if (end != std::string::npos) {
            result.push_back(copied_raw.substr(start, end - start));
        } else {
            result.push_back(copied_raw.substr(start));
            break;
        }
        copied_raw = copied_raw.substr(end);
    }
    if (!trailing.empty()) {
        result.push_back(trailing);
    }
    msg.setParams(result);
}
