#ifndef PARSER_HPP
#define PARSER_HPP

#include "Message.hpp"

class Parser {
 public:
  // 기본 생성자
  Parser();

  // 소멸자
  ~Parser();

  // 복사 생성자
  Parser(const Parser& other);

  // 대입 연산자
  Parser& operator=(const Parser& other);

  // 메시지 파싱 메서드
  Message parseMessage(const std::string& rawMessage) const;

 private:
  // helper 함수들은 const로 선언하여 객체의 상태를 변경하지 않음을 보장
  void parsePrefix(std::string& rawMessage, Message& msg) const;
  void parseCommand(std::string& rawMessage, Message& msg) const;
  void parseParameters(std::string& rawMessage, Message& msg) const;
  bool hasTrailing(std::string const& raw) const;

  static const char PREFIX_DELIMITER = ':';
  static const char PARAMETER_DELIMITER = ' ';
};

#endif  // PARSER_HPP