#pragma once
#include "PhdSocket.h"

//创建服务器socket
bool CreateServerSocket();

//接待线程
void ServerThread(void* p);

