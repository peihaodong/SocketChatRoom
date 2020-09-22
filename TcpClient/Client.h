#pragma once
#include "../TcpServer/PhdSocket.h"

//创建客户端socket
bool CreateClientSocket();

//用来接收消息的进程
void ReceiveThread(void* p);


