#include "CJson.h"
#include "json.h"


std::string CJson::CreateLogonJson(const std::string& strUser, const std::string& strPwd)
{
	/*
	User : (’À∫≈)
	Pwd : (√‹¬Î)
	*/
	Json::Value root;
	root["User"] = Json::Value(strUser);
	root["Pwd"] = Json::Value(strPwd);

	Json::StyledWriter sw;
	return sw.write(root);
}

void CJson::ParseLogonJson(const std::string& json, std::string& strUser, std::string& strPwd)
{
	Json::Value root;
	Json::Reader reader;
	reader.parse(json, root);
	strUser = root["User"].asString();
	strPwd = root["Pwd"].asString();
}

std::string CJson::CreateLoginJson(const std::string& strUser, const std::string& strPwd)
{
	/*
	User : (’À∫≈)
	Pwd : (√‹¬Î)
	*/
	Json::Value root;
	root["User"] = Json::Value(strUser);
	root["Pwd"] = Json::Value(strPwd);

	Json::StyledWriter sw;
	return sw.write(root);
}

void CJson::ParseLoginJson(const std::string& json, std::string& strUser, std::string& strPwd)
{
	Json::Value root;
	Json::Reader reader;
	reader.parse(json, root);
	strUser = root["User"].asString();
	strPwd = root["Pwd"].asString();
}

std::string CJson::CreateChatJson(const std::string& strUserName, const std::string& str)
{
	std::string strSum = strUserName;
	strSum += " Àµ£∫";
	strSum += str;
	return strSum;
}
