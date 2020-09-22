#pragma once
#include <string>
#include "json.h"

class PhdJson
{
	//************************************
	// Summary:   根节点键设置值
	// Update Time: 2020-9-8 10:44:33
	//************************************
	void RootKeySetValue(const std::string& strKey,const std::string& strValue);

	//************************************
	// Summary:  根节点键添加值
	// Update Time: 2020-9-8 11:01:11
	//************************************
	void RootKeyAppendValue(const std::string& strKey,const std::string& strValue);

	//************************************
	// Summary:  根节点键设置子节点
	// Update Time: 2020-9-8 11:01:56
	//************************************
	void RootKeySetChildNode(const std::string& strKey,const Json::Value& childNode);

	//************************************
	// Summary:  根节点数据写入json文件
	// Update Time: 2020-9-8 11:06:28
	//************************************
	bool WriteFileJson(LPCTSTR szFilePath);

	//////////////////////////////////////////////////////////////////////////

	//************************************
	// Summary:  json文件数据读取到根节点
	// Update Time: 2020-9-8 11:11:06
	//************************************
	bool ReadFileJson(LPCTSTR szFilePath);

	//************************************
	// Summary:  获取根节点键的值
	// Update Time: 2020-9-8 11:22:42
	//************************************
	bool GetValueOfRootKey(const std::string& strKey,std::string& strValue) const;

	//************************************
	// Summary:  获取根节点键的数组
	// Update Time: 2020-9-8 11:23:30
	//************************************
	bool GetValueArrayOfRootKey(const std::string& strKey,std::vector<std::string>& vecStr) const;

	


#pragma region 公用函数
	//************************************
	// Summary:  得到直接字符串
	// Update Time: 2020-9-7 18:06:50
	//************************************
	std::string GetFastString(const Json::Value& value);

	//************************************
	// Summary:  得到缩进字符串
	// Update Time: 2020-9-7 18:07:52
	//************************************
	std::string GetStyledString(const Json::Value& value);

	//************************************
	// Summary:  得到Json文件字符串
	// Update Time: 2020-9-7 18:17:23
	//************************************
	bool ReadFileJson(LPCTSTR szFilePath, std::string& strDocument);

	//************************************
	// Summary:  Json字符串写入文件中
	// Update Time: 2020-9-8 10:27:14
	//************************************
	bool WriteFileJson(const Json::Value& value, LPCTSTR szFilePath);

#pragma endregion

private:
	Json::FastWriter m_fw;
	Json::StyledWriter m_sw;
	Json::Reader m_reader;

	Json::Value m_valueRoot;
	Json::Value m_valueTemp;
	
};


