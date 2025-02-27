#include "utils.hpp"

namespace utils {
std::vector<std::string> split(std::string const &str, char delim) {
  std::vector<std::string> result;
  std::string token;
  std::string temp = str;

  while (!temp.empty()) {
    size_t end = temp.find(delim);
    if (end == std::string::npos) {
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

bool isNumber(std::string const &str) {
  for (size_t i = 0; i < str.size(); i++) {
    if (!std::isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool keventCompare(const struct kevent &lhs, const struct kevent &rhs) {
  return (lhs.ident == rhs.ident && lhs.filter == rhs.filter);
}
}  // namespace utils
