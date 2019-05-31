#ifndef COMMON_H
#define COMMON_H

#include <unordered_map>
#include <queue>
#include <string>

#include <QTcpSocket>

extern QTcpSocket *socket;
extern std::unordered_map<unsigned int, std::queue<std::string>> id_mq;

#endif // COMMON_H
