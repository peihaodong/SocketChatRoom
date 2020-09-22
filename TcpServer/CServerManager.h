#pragma once
#include "PhdSocket.h"
#include "CDbManager.h"

//服务器管理者
class CServerManager
{
public:
	CServerManager(PhdSocket* const pSocekt):m_pSocket(pSocekt){}

	bool Start();

private:
	//客户端注册请求
	void ClientLogonRequest();

	//客户端登录请求
	void ClientLoginRequest();

	//客户端聊天请求
	void ClientChatRequest();

private:
	PhdSocket* m_pSocket;
	CDbManager m_db;
};