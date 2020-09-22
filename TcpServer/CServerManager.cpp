#include "CServerManager.h"
#include <iostream>
#include "../TcpClient/CJson.h"
#include "CDbManager.h"
#include "define.h"
#include <map>

//每次数据传送的大小
#define DATASIZE  1024

extern std::map<PhdSocket*, std::string> g_mapSocket;

bool CServerManager::Start()
{
	//向客户端发送信息
	const char* pStr = "与服务器连接成功！";
	m_pSocket->Send(pStr, std::strlen(pStr));

	//阻塞循环接收信息
	while (true)
	{
		//接收协议编号
		int nProtocol = 0;
		int nCount = m_pSocket->Receive(&nProtocol, sizeof(nProtocol));
		if (nCount <= 0)
		{
			if (GetLastError() == 10054)
				std::cout << "客户端 " << *m_pSocket << " 退出了" << std::endl;
			break;
		}

		switch (nProtocol)
		{
		case NWPT_Logon:
			//客户端注册请求
			ClientLogonRequest();
			break;
		case NWPT_Login:
			//客户端登录请求
			ClientLoginRequest();
			break;
		case NWPT_Chat:
			//客户端聊天请求
			ClientChatRequest();
			break;
		default:
			break;
		}
	}

	return true;
}

void CServerManager::ClientLogonRequest()
{
	//接收json字符串大小
	int nJsonSize = 0;
	m_pSocket->Receive(&nJsonSize, sizeof(nJsonSize));
	//接收json字符串
	char* pJson = new char[nJsonSize+1];
	std::memset(pJson,0,nJsonSize+1);
	m_pSocket->Receive(pJson, nJsonSize + 1);
	std::string strJson = pJson;
	delete[] pJson;
	//解析json字符串
	std::string strUser, strPwd;
	CJson::ParseLogonJson(strJson, strUser, strPwd);

	//添加用户
	if (m_db.AddUser(strUser, strPwd))
	{//注册成功
		//发送协议编号
		int Protocol = NWPT_Logon;
		m_pSocket->Send(&Protocol, sizeof(Protocol));
		//发送注册成功信息
		int nTrue = RT_TRUE;
		m_pSocket->Send(&nTrue, sizeof(nTrue));
	}
	else
	{//注册失败
		//发送协议编号
		int Protocol = NWPT_Logon;
		m_pSocket->Send(&Protocol, sizeof(Protocol));
		//发送注册失败信息
		int nFalse = RT_FALSE;
		m_pSocket->Send(&nFalse, sizeof(nFalse));
	}
}

void CServerManager::ClientLoginRequest()
{
	//接收json字符串大小
	int nJsonSize = 0;
	m_pSocket->Receive(&nJsonSize, sizeof(nJsonSize));
	//接收json字符串
	char* pJson = new char[nJsonSize + 1];
	std::memset(pJson, 0, nJsonSize + 1);
	m_pSocket->Receive(pJson, nJsonSize + 1);
	std::string strJson = pJson;
	delete[] pJson;
	//解析json字符串
	std::string strUser, strPwd;
	CJson::ParseLoginJson(strJson, strUser, strPwd);

	//用户是否存在
	if (m_db.InfoIsCorrect(strUser, strPwd))
	{//存在
		g_mapSocket[m_pSocket] = strUser;//给socket赋值用户名
		//发送协议编号
		int Protocol = NWPT_Login;
		m_pSocket->Send(&Protocol, sizeof(Protocol));
		//发送登录成功信息
		int nTrue = RT_TRUE;
		m_pSocket->Send(&nTrue, sizeof(nTrue));
	}
	else
	{//不存在
		//发送协议编号
		int Protocol = NWPT_Login;
		m_pSocket->Send(&Protocol, sizeof(Protocol));
		//发送登录失败信息
		int nFalse = RT_FALSE;
		m_pSocket->Send(&nFalse, sizeof(nFalse));
	}
}

void CServerManager::ClientChatRequest()
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

	//发送给所有在线客户端
	for (auto iter = g_mapSocket.begin(); iter != g_mapSocket.end(); iter++)
	{
		if (iter->second != "")
		{
			//发送协议编号
			int Protocol = NWPT_Chat;
			iter->first->Send(&Protocol, sizeof(Protocol));
			//发送字符串大小
			int nStrSize = str.length();
			iter->first->Send(&nStrSize, sizeof(nStrSize));
			//发送字符串
			iter->first->Send(str.c_str(), str.length());
		}
	}
}
