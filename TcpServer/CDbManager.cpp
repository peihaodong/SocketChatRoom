#include "stdafx.h"
#include "CDbManager.h"
#include "CAdoLx.h"


extern CAdoLx* g_pAdo;

bool CDbManager::AddUser(const std::string& strUser, const std::string& Pwd) const
{
	CString strSQL;
	strSQL.Format("SELECT * FROM t_UserInfo WHERE f_User = '%s'",strUser.c_str());
	//存在的话，返回false
	g_pAdo->Select(strSQL);
	int nCount = g_pAdo->GetRecordCount();
	if (nCount)
		return false;
	//数据库增加数据
	strSQL.Format("INSERT INTO t_UserInfo VALUES('%s','%s')", strUser.c_str(), Pwd.c_str());
	if (g_pAdo->ExecSQL(strSQL) < 0)
		return false;
	else
		return true;
}

bool CDbManager::InfoIsCorrect(const std::string& strUser, const std::string& Pwd) const
{
	CString strSQL;
	strSQL.Format("SELECT * FROM t_UserInfo WHERE f_User = '%s'", 
		strUser.c_str());
	//不存在的话，返回false
	g_pAdo->Select(strSQL);
	int nCount = g_pAdo->GetRecordCount();
	if (!nCount)
		return false;
	CString strNewPwd;
	if (!g_pAdo->GetFieldByIndex(1, strNewPwd))
		return false;
	if (std::strcmp(Pwd.c_str(), strNewPwd) == 0)
		return true;
	else
		return false;
}
