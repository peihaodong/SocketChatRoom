#include "CClientRecvManager.h"
#include "../TcpServer/define.h"
#include <iostream>
#include <list>

extern bool g_bWaitFlag;
extern int g_nReturn;
extern std::list<std::string> g_strChat;	//聊天记录
extern int g_nDlgType;		//界面类型 


bool CClientRecvManager::Start()
{
	while (true)
	{
		int nProtocol = 0;
		m_pSocket->Receive(&nProtocol, sizeof(nProtocol));

		switch (nProtocol)
		{
		case NWPT_Logon:
			//服务器回复的注册结果信息
			ServerLogonReply();
			break;
		case NWPT_Login:
			//服务器回复的登录结果信息
			ServerLoginReply();
			break;
		case NWPT_Chat:
			//服务器回复的聊天信息
			ServerChatReply();
			break;
		default:
			break;
		}
	}
}

void CClientRecvManager::ServerLogonReply()
{
	int nRt = 0;
	m_pSocket->Receive(&nRt, sizeof(nRt));
	g_nReturn = nRt;
	g_bWaitFlag = true;
	
}

void CClientRecvManager::ServerLoginReply()
{
	int nRt = 0;
	m_pSocket->Receive(&nRt, sizeof(nRt));
	g_nReturn = nRt;
	g_bWaitFlag = true;
}

void CClientRecvManager::ServerChatReply()
{
	//接收字符串大小
	int nSize = 0;
	m_pSocket->Receive(&nSize, sizeof(nSize));
	//接收字符串
	char* pStr = new char[nSize + 1];
	std::memset(pStr, 0, nSize + 1);
	m_pSocket->Receive(pStr, nSize + 1);
	std::string str = pStr;
	delete[] pStr;

	//添加到聊天记录中
	g_strChat.push_back(str);

	//清屏，重新输入聊天记录
	if (g_nDlgType == 5)
	{
		std::system("cls");
		for (auto iter = g_strChat.begin(); iter != g_strChat.end(); iter++)
		{
			std::cout << *iter << std::endl;
		}
		std::cout << std::endl;
	}
	else if (g_nDlgType == 4 || g_nDlgType == 6 || g_nDlgType ==7)
	{
		std::cout << "收到一条信息" << std::endl;
	}

	g_bWaitFlag = true;
}
