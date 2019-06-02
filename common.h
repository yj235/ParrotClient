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

extern QTcpSocket *socket;
extern std::unordered_map<unsigned int, std::queue<Time_Message>> id_mq;

void socket_write(const std::string &str);

#endif // COMMON_H
