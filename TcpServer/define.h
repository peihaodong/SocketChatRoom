#pragma once

//每次数据传送的大小
#define DATASIZE  1024

//协议号
enum EProtocol
{
	RT_FALSE = 0,		//返回失败
	RT_TRUE,			//返回成功

	NWPT_Logon = 10,	//注册
	NWPT_Login,			//登录
	NWPT_Chat,			//聊天
	NWPT_UploadFile,	//上传文件
	DownloadFile		//下载文件
};

