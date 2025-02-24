#include "UserRepository.hpp"

UserRepository *UserRepository::instance = NULL;

UserRepository::UserRepository(){}

UserRepository &UserRepository::getInstance()
{
  if(instance == NULL)
  {
    instance = new UserRepository();
    instance->_users = std::map<int, User>();
  }
  return *instance;
}

void UserRepository::destroy()
{
  if(instance != NULL)
  {
    instance->_users.clear();
    delete instance;
  }
}

void UserRepository::addUser(User user)
{
  _users[user.getSocket()] = User(user);
}

void UserRepository::removeUser(User & user)
{
  _users.erase(user.getSocket());
}
void UserRepository::removeUser(int clientSocket)
{
  _users.erase(clientSocket);
}

User *UserRepository::getUser(std::string nickname)
{
  for(std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
  {
    if(it->second.getNickname() == nickname)
    {
      return &it->second;
    }
  }
  return NULL;
}

User *UserRepository::getUser(int clientSocket)
{
  if(_users.find(clientSocket) != _users.end()){
    return &_users[clientSocket];
  }
  return NULL;
}

std::map<int,User> &UserRepository::getUsers()
{
  return _users;
}

bool UserRepository::hasUser(std::string nickname)
{
  if (getUser(nickname) != NULL)
  {
    return true;
  }
  return false;
}

UserRepository::~UserRepository()
{
}

