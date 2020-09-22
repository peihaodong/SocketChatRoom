#include "Server.h"
#include <iostream>
#include "../TcpClient/CJson.h"
#include <list>
#include <algorithm>
#include "CServerManager.h"
#include <map>


//全局socket容器（key：socket指针，value：用户名）
std::map<PhdSocket*, std::string> g_mapSocket;


bool CreateServerSocket()
{
	//创建socket
	PhdSocket bossSocket;
	if (!bossSocket.Create(8080))
	{
		std::perror("socket create");
		std::exit(0);
	}
	//监听
	if (!bossSocket.Listen())
	{
		std::perror("socket listen");
		std::exit(0);
	}

	std::cout << "服务器已开启......" << std::endl;

	while (true)
	{
		//等待客户端连接
		PhdSocket* pServerSocket = new PhdSocket;
		std::string strIP;
		UINT nPort = 0;
		//accept 阻塞函数 如果没有客户端连接，一直等待客户端连接
		if (!bossSocket.Accept(pServerSocket, strIP, &nPort))
		{
			std::perror("socket accept");
			continue;
		}
		std::cout << "IP地址：" << strIP << " 端口号：" << nPort << " 的客户登录了。" << std::endl;

		//启动线程
		_beginthread(ServerThread, 0, pServerSocket);
	}

	return true;
}

void ServerThread(void* p)
{
	PhdSocket* pServerSocket = (PhdSocket*)p;
	//添加socket到容器中
	g_mapSocket[pServerSocket] = "";

	//创建服务器管理者对象
	CServerManager manager(pServerSocket);
	manager.Start();
	
	//从容器中删除该socket
	auto iter = g_mapSocket.find(pServerSocket);
	if (iter != g_mapSocket.end())
		g_mapSocket.erase(iter);

	//释放接待socket
	delete pServerSocket;
}


