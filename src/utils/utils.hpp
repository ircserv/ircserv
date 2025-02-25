#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <iostream>

namespace utils {
  std::vector<std::string> split(const std::string &str, char delim);
  bool isNumber(const std::string &str);

  struct KeventCompare {
    bool operator()(const struct kevent& k1, const struct kevent& k2) const {
      return k1.ident < k2.ident;
    }
  };
}

#endif 