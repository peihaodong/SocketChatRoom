#pragma once
#include <string>

//数据库管理者
class CDbManager
{
public:

	//添加用户
	bool AddUser(const std::string& strUser,const std::string& Pwd) const;

	//用户账号密码是否正确
	bool InfoIsCorrect(const std::string& strUser, const std::string& Pwd) const;

};