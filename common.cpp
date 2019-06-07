#include "common.h"

//ID
unsigned int ID;

//socket
QTcpSocket *socket;

//本地用户消息队列;
std::unordered_map<unsigned int, std::queue<Time_Message>> contacts_mq;

//本地群消息队列
std::unordered_map<unsigned int, std::queue<ID_Time_Message>> group_mq;

void socket_write(const std::string &str){
    unsigned int len = str.length();
    char data[1024] = {0};
    memcpy(data, &len, sizeof(len));
    memcpy(data + sizeof(len), str.c_str(), len);
    socket->write(data, sizeof(len) + len);
}
