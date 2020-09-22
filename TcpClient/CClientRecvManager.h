#pragma once
#include "../TcpServer/PhdSocket.h"

//客户端信息接收管理者
class CClientRecvManager
{
public:
	CClientRecvManager(PhdSocket* const pSocket):m_pSocket(pSocket){}

	bool Start();

private:
	//服务器注册回复
	void ServerLogonReply();

	//服务器回复的登录结果信息
	void ServerLoginReply();

	//服务器回复的聊天信息
	void ServerChatReply();

private:
	PhdSocket* m_pSocket;
};


