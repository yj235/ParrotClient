#include "common.h"

QTcpSocket *socket;

//本地消息队列 id_queue;
std::unordered_map<unsigned int, std::queue<std::string>> id_mq;
