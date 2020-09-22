#include "Client.h"
#include <iostream>
#include "../TcpServer/define.h"
#include "CClientDlgManager.h"
#include "CClientRecvManager.h"
#include <list>

//操作两个线程的全局变量
bool g_bWaitFlag;
int g_nReturn;

std::list<std::string> g_strChat;	//聊天记录
int g_nDlgType;		//界面类型 

bool CreateClientSocket()
{
	//创建socket
	PhdSocket* pClientSocket = new PhdSocket;
	if (!pClientSocket->Create())
	{
		std::perror("socket create");
		std::exit(0);
	}

	if (!pClientSocket->Connect("192.168.1.100", 8080))
	{
		std::perror("socket connect");
		std::exit(0);
	}

	char strData[DATASIZE] = { 0 };
	//接收服务器发来的信息
	int n = pClientSocket->Receive(strData, DATASIZE);
	std::cout << strData << std::endl;

 	//创建接收线程
 	_beginthread(ReceiveThread, 0, pClientSocket);

	//客户端界面管理者
	CClientDlgManager dlg(pClientSocket);
	dlg.Start();

	delete pClientSocket;
	return true;
}

void ReceiveThread(void* p)
{
	PhdSocket* pClientSocket = (PhdSocket*)p;

	//客户端接收信息管理者
	CClientRecvManager recv(pClientSocket);
	recv.Start();
	
}


