#include "IRCServer.hpp"

void onConnect(fd clientSocket, void* message){
  
  // IRCServer* ircServer = static_cast<IRCServer*>(server);
  (void)message;
  IRCServer &ircServer = IRCServer::getInstance();
  ircServer.send(clientSocket, "Welcome to the IRC Server");
}



int main(){
  IRCServer &server = IRCServer::getInstance();
  server.on("JOIN", onConnect);
  server.start(6667);
  server.destroy();
  return 0;
}