#include <iostream>
#include "stdafx.h"
#include "CAdoLx.h"
#include "Server.h"

//全局变量-数据库指针
CAdoLx* g_pAdo;


int main()
{
	//连接数据库
	g_pAdo = new CAdoLx;
	if (!g_pAdo->Connect(CAdoLx::DBT_SQL,"PhdDb","phd064315","peihaodong","192.168.1.100","1433"))
	{
		std::perror("Db Connect");
		std::exit(0);
	}

	//创建服务器socket
	CreateServerSocket();

	g_pAdo->Release();
	g_pAdo->Close();
	delete g_pAdo;
	return 0;
}


