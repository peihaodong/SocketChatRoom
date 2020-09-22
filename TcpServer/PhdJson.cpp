#include "stdafx.h"
#include "PhdJson.h"
#include <fstream>

void PhdJson::RootKeySetValue(const std::string& strKey, const std::string& strValue)
{
	m_valueRoot[strKey] = Json::Value(strValue);
}

void PhdJson::RootKeyAppendValue(const std::string& strKey, const std::string& strValue)
{
	m_valueRoot[strKey].append(strValue);
}

void PhdJson::RootKeySetChildNode(const std::string& strKey, const Json::Value& childNode)
{
	m_valueRoot[strKey] = Json::Value(childNode);
}

std::string PhdJson::GetFastString(const Json::Value& value) 
{
	return m_fw.write(value);
}

std::string PhdJson::GetStyledString(const Json::Value& value)
{
	return m_sw.write(value);
}

bool PhdJson::ReadFileJson(LPCTSTR szFilePath,std::string& strDocument)
{
	std::ifstream fileIn;		//输入流（读取文件数据）
	//打开文件
	fileIn.open(szFilePath, std::ios::binary);
	if (!fileIn.is_open())
		return false;
	
	//读取文件
	m_reader.parse(fileIn, m_valueTemp);
	strDocument = m_sw.write(m_valueTemp);

	//关闭文件
	fileIn.clear();
	fileIn.close();

	return true;
}

bool PhdJson::ReadFileJson(LPCTSTR szFilePath)
{
	std::ifstream fileIn;		//输入流（读取文件数据）
	//打开文件
	fileIn.open(szFilePath, std::ios::binary);
	if (!fileIn.is_open())
		return false;

	//读取文件
	m_reader.parse(fileIn, m_valueRoot);

	//关闭文件
	fileIn.clear();
	fileIn.close();

	return true;
}

bool PhdJson::GetValueOfRootKey(const std::string& strKey, std::string& strValue) const
{
	strValue = m_valueRoot[strKey].asString();
	return true;
}

bool PhdJson::GetValueArrayOfRootKey(const std::string& strKey, std::vector<std::string>& vecStr) const
{
	for (unsigned int i = 0; i < m_valueRoot[strKey].size(); i++)
	{
		vecStr.push_back(m_valueRoot[strKey][i].asString());
	}
	return true;
}



bool PhdJson::WriteFileJson(const Json::Value& value, LPCTSTR szFilePath)
{
	std::ofstream fileOn;
	fileOn.open(szFilePath, std::ios::binary | std::ios::app);
	if (!fileOn.is_open())
		return false;
	
	fileOn << m_sw.write(value);

	fileOn.clear();
	fileOn.close();
	return true;
}

bool PhdJson::WriteFileJson(LPCTSTR szFilePath)
{
	std::ofstream fileOn;
	fileOn.open(szFilePath, std::ios::binary | std::ios::app);
	if (!fileOn.is_open())
		return false;

	fileOn << m_sw.write(m_valueRoot);

	fileOn.clear();
	fileOn.close();
	return true;
}

