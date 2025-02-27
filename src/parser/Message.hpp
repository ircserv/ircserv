#ifndef IRC_MESSAGE_HPP
#define IRC_MESSAGE_HPP

#include <string>
#include <vector>
class Message {
 public:
  // 기본 생성자
  Message();

  // 매개변수가 있는 생성자
  explicit Message(const std::string& raw);

  // 소멸자
  ~Message();

  // 복사 생성자
  Message(const Message& other);

  // 대입 연산자
  Message& operator=(const Message& other);

  // getter/setter
  const std::string& getPrefix() const;
  const std::string& getCommand() const;
  const std::vector<std::string>& getParams() const;
  const std::string& getRawMessage() const;

  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  /// @brief
  /// @param params
  void setParams(const std::vector<std::string>& params);
  void setRawMessage(const std::string& rawMessage);

 private:
  std::string rawMessage;
  std::string prefix;
  std::string command;
  std::vector<std::string> params;
};

#endif  // IRC_MESSAGE_HPP