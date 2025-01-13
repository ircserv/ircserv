#include "server/Server.hpp"

int main() {

    Server *server = Server::getInstance();
    server->setPort(8080);
    server->listen();
    return 0;
}