#include "Message.hpp"

// Message 클래스 구현
Message::Message() : rawMessage(""), prefix(""), command(""), params() {}

Message::Message(const std::string& raw)
    : rawMessage(raw), prefix(""), command(""), params() {}

Message::~Message() {}

Message::Message(const Message& other)
    : rawMessage(other.rawMessage),
      prefix(other.prefix),
      command(other.command),
      params(other.params) {}

Message& Message::operator=(const Message& other) {
  if (this != &other) {
    rawMessage = other.rawMessage;
    prefix = other.prefix;
    command = other.command;
    params = other.params;
  }
  return *this;
}

const std::string& Message::getPrefix() const { return prefix; }
const std::string& Message::getCommand() const { return command; }
const std::vector<std::string>& Message::getParams() const { return params; }
const std::string& Message::getRawMessage() const { return rawMessage; }

void Message::setPrefix(const std::string& prefix) { this->prefix = prefix; }

void Message::setCommand(const std::string& command) {
  this->command = command;
}

void Message::setParams(const std::vector<std::string>& params) {
  this->params = params;
}