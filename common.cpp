#include "common.h"

QTcpSocket *socket;

//本地消息队列 id_queue;
std::unordered_map<unsigned int, std::queue<Time_Message>> id_mq;

void socket_write(const std::string &str){
    unsigned int len = str.length();
    char data[1024] = {0};
    memcpy(data, &len, sizeof(len));
    memcpy(data + sizeof(len), str.c_str(), len);
    socket->write(data, sizeof(len) + len);
}
