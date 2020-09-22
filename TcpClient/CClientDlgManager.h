#pragma once
#include "../TcpServer/PhdSocket.h"

//客户端界面管理者
class CClientDlgManager
{
public:
	CClientDlgManager(PhdSocket* const pSocket):m_pSocket(pSocket){}

	bool Start();

private:
	//打开界面(1)
	void DlgOpen();

	//注册界面(2)
	void DlgLogon();

	//登录界面(3)
	void DlgLogin();

	//客户端界面(4)
	void DlgClient();

	//聊天(5)
	void Chat();
	//上传文件(6)
	void UploadFile();
	//下载文件(7)
	void DownloadFile();

private:
	//判断该文件是否存在，返回文件大小
	bool FileIsExist(const std::string& strFile, unsigned long long& size) const;

private:
	PhdSocket* m_pSocket;
	std::string m_strUserName;
};