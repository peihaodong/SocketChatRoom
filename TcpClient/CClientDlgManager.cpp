#include "CClientDlgManager.h"
#include <iostream>
#include "../TcpServer/define.h"
#include "CJson.h"
#include <list>
#include <fstream>

extern bool g_bWaitFlag;
extern int g_nReturn;
extern std::list<std::string> g_strChat;	//聊天记录
extern int g_nDlgType;		//界面类型 

bool CClientDlgManager::Start()
{
	//打开界面
	DlgOpen();

	return true;
}

void CClientDlgManager::DlgOpen()
{
	while (true)
	{
		g_nDlgType = 1;
		std::system("cls");

		std::cout << "欢迎登录裴浩东的聊天室" << std::endl;
		std::cout << std::endl;
		std::cout << "1：登录" << std::endl;
		std::cout << "2：注册" << std::endl;
		std::cout << "0：退出" << std::endl;
		std::cout << std::endl;
		int nTemp = 0;
		std::cout << "请输入对应序号：";
		std::cin >> nTemp;

		if (nTemp == 1)
		{
			//登录界面
			DlgLogin();
		}
		else if (nTemp == 2)
		{
			//注册界面
			DlgLogon();
		}
		else if (nTemp == 0)
			return;
		else
		{
			std::cout << "你输入的序号不对，执行自动毁灭模式。。。" << std::endl;
			std::getchar();
			return;
		}
	}
}

void CClientDlgManager::DlgLogon()
{
	while (true)
	{
		g_nDlgType = 2;
		std::system("cls");

		std::cout << "注册界面" << std::endl;
		std::cout << std::endl;
		std::cout << "0：返回" << std::endl;
		std::cout << std::endl;
		std::cout << "输入账号：";
		std::string strUser, strPwd;
		std::cin >> strUser;
		std::cout << "输入密码：";
		std::cin >> strPwd;
		std::cout << std::endl;
		if (strUser == "0" || strPwd == "0")
			return;
		//发送协议编号
		int nProtocol = NWPT_Logon;
		m_pSocket->Send(&nProtocol, sizeof(nProtocol));
		//处理成json字符串
		std::string strJson = CJson::CreateLogonJson(strUser, strPwd);
		//发送json大小
		int nJsonSize = strJson.length();
		m_pSocket->Send(&nJsonSize, sizeof(nJsonSize));
		//json字符串发送给服务器
		m_pSocket->Send(strJson.c_str(), strJson.length());

		g_bWaitFlag = false;
		while (!g_bWaitFlag);

		if (g_nReturn)
		{
			std::cout << "注册成功" << std::endl;
			std::rewind(stdin);	//清理缓存
			std::getchar();		//暂停
			return;
		}
		else
		{
			std::cout << "注册失败" << std::endl;
			std::rewind(stdin);	//清理缓存
			std::getchar();		//暂停
		}
	}
}

void CClientDlgManager::DlgLogin()
{
	while (true)
	{
		g_nDlgType = 3;
		std::system("cls");

		std::cout << "登录界面" << std::endl;
		std::cout << std::endl;
		std::cout << "0：返回" << std::endl;
		std::cout << std::endl;
		std::cout << "输入账号：";
		std::string strUser, strPwd;
		std::cin >> strUser;
		std::cout << "输入密码：";
		std::cin >> strPwd;
		std::cout << std::endl;
		if (strUser == "0" || strPwd == "0")
			return;
		//发送协议编号
		int nProtocol = NWPT_Login;
		m_pSocket->Send(&nProtocol, sizeof(nProtocol));
		//处理成json字符串
		std::string strJson = CJson::CreateLoginJson(strUser, strPwd);
		//发送json大小
		int nJsonSize = strJson.length();
		m_pSocket->Send(&nJsonSize, sizeof(nJsonSize));
		//json字符串发送给服务器
		m_pSocket->Send(strJson.c_str(), strJson.length());

		g_bWaitFlag = false;
		while (!g_bWaitFlag);

		if (g_nReturn)
		{//登录成功
			std::cout << "登录成功" << std::endl;
			std::rewind(stdin);	//清理缓存
			std::getchar();		//暂停
			m_strUserName = strUser;
			//客户端界面
			DlgClient();
		}
		else
		{//登录失败
			std::cout << "登录失败" << std::endl;
			std::rewind(stdin);	//清理缓存
			std::getchar();		//暂停
		}
	}
}

void CClientDlgManager::DlgClient()
{
	while (true)
	{
		g_nDlgType = 4;
		std::system("cls");

		std::cout << "用户界面" << std::endl;
		std::cout << std::endl;
		std::cout << "1：聊天" << std::endl;
		std::cout << "2：上传文件" << std::endl;
		std::cout << "3：下载文件" << std::endl;
		std::cout << "0：返回" << std::endl;
		std::cout << std::endl;
		std::cout << "输入序号：";
		int nNumber = 0;
		std::cin >> nNumber;

		switch (nNumber)
		{
		case 1:
			//聊天
			Chat();
			break;
		case 2:
			//上传文件
			//UploadFile();
			break;
		case 3:
			//下载文件
			//DownloadFile();
			break;
		case 0:
			return;
		default:
			std::cout << "你输入的序号不对，执行自动毁灭模式。。。" << std::endl;
			std::getchar();
			return;
		}
	}
}

void CClientDlgManager::Chat()
{
	//清屏，刷新聊天信息
	std::system("cls");
	for (auto iter = g_strChat.begin(); iter != g_strChat.end(); iter++)
	{
		std::cout << *iter << std::endl;
	}
	std::cout << std::endl;

	while (true)
	{
		g_nDlgType = 5;

		std::cout << "输入你要说的话[#退出]：";
		std::string str;
		std::cin >> str;
		if (str[0] == '#')
			break;

		//发送协议编号
		int nProtocol = NWPT_Chat;
		m_pSocket->Send(&nProtocol, sizeof(nProtocol));
		//处理成json字符串
		std::string strJson = CJson::CreateChatJson(m_strUserName, str);
		//发送json大小
		int nJsonSize = strJson.length();
		m_pSocket->Send(&nJsonSize, sizeof(nJsonSize));
		//json字符串发送给服务器
		m_pSocket->Send(strJson.c_str(), strJson.length());

		g_bWaitFlag = false;
		while (!g_bWaitFlag);
	}
}

// void CClientDlgManager::UploadFile()
// {
// 	while (true)
// 	{
// 		g_nDlgType = 6;
// 
// 		std::cout << "输入你要上传的文件（绝对路径）[#退出]：";
// 		std::string strFile;
// 		std::cin >> strFile;
// 		if (strFile[0] == '#')
// 			break;
// 
// 		//判断该文件是否存在，返回文件大小
// 		unsigned long long size = 0;
// 		if (!FileIsExist(strFile, size))
// 		{
// 			std::cout << "该文件不存在，请查看" << std::endl;
// 			continue;
// 		}
// 
// 		//发送协议编号
// 		int nProtocol = NWPT_UploadFile;
// 		m_pSocket->Send(&nProtocol, sizeof(nProtocol));
// 		//处理成json字符串
// 		std::string strJson = CJson::CreateUploadFileJson(strFile, size);
// 		//发送json大小
// 		int nJsonSize = strJson.length();
// 		m_pSocket->Send(&nJsonSize, sizeof(nJsonSize));
// 		//json字符串发送给服务器
// 		m_pSocket->Send(strJson.c_str(), strJson.length());
// 
// 		g_bWaitFlag = false;
// 		while (!g_bWaitFlag);
// 	}
// }

bool CClientDlgManager::FileIsExist(const std::string& strFile,unsigned long long& size) const
{
	std::ifstream fileIn;
	fileIn.open(strFile.c_str(), std::ios::binary, std::ios::_Nocreate);
	if (!fileIn.is_open())
		return false;

	fileIn.seekg(0, std::ios::end);
	size = fileIn.tellg();
	fileIn.seekg(0, std::ios::beg);

	fileIn.close();
	return true;
}
