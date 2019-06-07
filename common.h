#ifndef COMMON_H
#define COMMON_H

#include <QTcpSocket>

#include <unordered_map>
#include <queue>
#include <string>

#include <string.h>

typedef struct Time_Message{
    std::string time;
    std::string message;
}Time_Message;

typedef struct ID_Time_Message{
    unsigned int id;
    std::string time;
    std::string message;
}ID_Time_Message;

//ID
extern unsigned int ID;
//端口
extern QTcpSocket *socket;
//联系人消息队列
extern std::unordered_map<unsigned int, std::queue<Time_Message>> contacts_mq;
//群消息队列
extern std::unordered_map<unsigned int, std::queue<ID_Time_Message>> group_mq;

void socket_write(const std::string &str);

#endif // COMMON_H
