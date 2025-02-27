#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include <map>
#include <string>

#include "User.hpp"

class User;
class UserRepository {
 public:
  static UserRepository &getInstance();
  static void destroy();

  void addUser(int socket);
  void removeUser(int socket);
  User *getUser(std::string nickname);
  User *getUser(int clientSocket);
  std::map<int, User> &getUsers();
  bool hasUser(std::string nickname);

 private:
  static UserRepository *instance;
  std::map<int, User> _users;

  UserRepository();
  ~UserRepository();
  UserRepository(const UserRepository &other);
  UserRepository &operator=(const UserRepository &other);
};

#endif