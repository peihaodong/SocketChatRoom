#pragma once
#include <string>

//生成和解析json数据的封装类
class CJson 
{
public:

	//注册
	static std::string CreateLogonJson(const std::string& strUser,const std::string& strPwd);
	static void ParseLogonJson(const std::string& json,std::string& strUser,std::string& strPwd);

	//登录
	static std::string CreateLoginJson(const std::string& strUser, const std::string& strPwd);
	static void ParseLoginJson(const std::string& json, std::string& strUser, std::string& strPwd);

	//聊天
	static std::string CreateChatJson(const std::string& strUserName,const std::string& str);

};