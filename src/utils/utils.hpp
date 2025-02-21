#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <vector>

namespace utils {
  std::vector<std::string> split(const std::string &str, char delim);
  bool isNumber(const std::string &str);
} // namespace utils


#endif 