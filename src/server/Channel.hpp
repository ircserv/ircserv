#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include "User.hpp"
#include <set>

#define CHSYMBOL_PUBLIC "="
#define CHSYMBOL_SECRET "@"

enum CH_MODE {
  MODE_INVITE = 1, // - i : 초대만 가능한 채널로 설정/행제
  MODE_TOPIC = 2, // - t:  topic명령어를 채널의 관리자로 제한 설정/해제
  MODE_KEY  = 4, // - k: 채널의 패스워드 설정/해제
  MODE_OPERATOR = 8, // - o: 채널의 관리자 설정/해제
  MODE_LIMIT = 16, // - l: 채널의 유저제한 설정/해제
};

typedef std::string channelName;

class User;

class Channel {
  private:
    std::string name;
    std::string symbol;
    std::map<int, User *> users;
    std::set<std::string> chops;
    std::string key;
    size_t capacity;
    std::string topic;
    char mode;


  public:
    Channel();
    Channel(std::string name, User &user);
    ~Channel();
    Channel(const Channel & other);
    Channel & operator=(const Channel & other);
    void join(User &user);
    void part(User &user);
    void send(User &user, std::string message);
    void kick(User &user);
    void broadcast(std::string message);
    void toOperators(User &sender, std::string const &message);

    std::vector<User *> getUsers();
    const std::string &getName();
    const std::string &getSymbol();
    const std::string &getTopic();
    bool hasUser(User &user);
    bool isFull();
    bool isInviteOnly();
    bool isKeyProtected();
    bool isOperator(User &user);
    bool isLimit();
    bool authenticate(std::string key);
    void setMode(std::string mode, std::vector<std::string> keys);
    
    void sendBufferFlush();
};


#endif