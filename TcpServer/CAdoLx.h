// AdoLx.h: Version 1.1（支持Unicode、支持VS2015和VC6编译环境）
//支持Unicode、支持Access、SQLServer和Oracle三大数据库
//Author:吕鑫老师封装类 
//吕鑫老师微博：http://t.qq.com/richmain 吕鑫老师微信：richmain
//更多更好的是项目开发视频教程，请登录http://www.baojy.com
//////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning (disable:4146)
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")rename ("EOS","adoEOS")
//#import "E:\A我的笔记\PhdNotes\Sql\msado15.dll" no_namespace rename ("EOF","EndOfFile") rename ("EOS","EndOfStream")


class CAdoLx
{
	_RecordsetPtr m_pRst;		//记录对象智能指针
	_ConnectionPtr m_pConn;		//连接对象智能指针

	CString m_szLastError;
public:
	CAdoLx();
	virtual ~CAdoLx();

	void Release()
	{
		m_pConn->Release();
	}
	void Close()
	{
		m_pConn->Close();
	}

	BOOL GetFieldType(long nIndex, int &nType);

	//连接数据库的类型
	enum EType { DBT_ACCESS, DBT_SQL, DBT_ORACLE };

	
	// Summary:   连接数据库
	// Time:	  2020年3月21日 peihaodong
	// Explain:	  数据库类型；数据库名；密码；用户名；ip地址；端口号（1433）
	BOOL Connect(EType eType, LPCTSTR szDatabase, LPCTSTR szPass = NULL, LPCTSTR szUser = NULL, LPCTSTR szHost = NULL,LPCTSTR szPort = NULL);

	//增删改插入数据
	int ExecSQL(LPCTSTR szSQL);

	//查询数据
	BOOL Select(LPCTSTR szSQL);

	//遍历数据
	BOOL IsEOF();
	BOOL MoveNext();
	BOOL MovePrev();

	BOOL MoveFirst();
	BOOL MoveLast();

	//得到字段数量
	int GetFieldCount();
	//通过下标得到字段名
	BOOL GetFieldName(long nIndex, CString &szName);
	//得到记录数量
	int GetRecordCount();
	//得到错误信息
	CString GetLastError()
	{
		CString str = m_szLastError;
		m_szLastError.Empty();
		return str;
	}

	//根据字段名获取数据内容
	BOOL GetFieldValue(LPCTSTR szField, COleDateTime &tValue);
	BOOL GetFieldValue(LPCTSTR szField, double &fValue);
	BOOL GetFieldValue(LPCTSTR szField, CString &sValue);
	BOOL GetFieldValue(LPCTSTR szField, int &iValue);

	//根据索引获取数据内容
	BOOL GetFieldByIndex(long nIndex, COleDateTime &tValue);
	BOOL GetFieldByIndex(long nIndex, double &fValue);
	BOOL GetFieldByIndex(long nIndex, CString &sValue);
	BOOL GetFieldByIndex(long nIndex, int &iValue);
	BOOL GetFieldByIndex(long nIndex, long &lValue);
	BOOL GetFieldByIndex(long nIndex, DWORD &dwValue);
	BOOL GetFieldByIndex(long nIndex, UINT &iValue);
	BOOL GetFieldByIndex(long nIndex, char &cValue);
	BOOL GetFieldByIndex(long nIndex, short &nValue);
	BOOL GetFieldByIndex(long nIndex, BYTE &cbValue);
	BOOL GetFieldByIndex(long nIndex, WORD &wValue);
	BOOL GetFieldByIndex(long nIndex, TCHAR sValue[], int nSize);

	class CProc
	{
		/*
		enum ParameterDirectionEnum
		{
		adParamUnknown = 0,
		adParamInput = 1,
		adParamOutput = 2,
		adParamInputOutput = 3,
		adParamReturnValue = 4
		};
		*/
		CString m_szLastError;
		_CommandPtr m_pCmd;
		//_ConnectionPtr & m_pConn;
	public:
		LPCTSTR GetLastError() const
		{
			return m_szLastError;
		}
		int Exec();
		BOOL Create(_ConnectionPtr &pConn, LPCTSTR szProc);

		BOOL AddParam(LPCTSTR szVName, long lParam, ParameterDirectionEnum eDir = adParamInput);
		BOOL AddParam(LPCTSTR szVName, LPCTSTR szParam, ParameterDirectionEnum eDir = adParamInput);

		BOOL GetValue(LPCTSTR szVName, long &lParam);
		BOOL GetValue(LPCTSTR szVName, CString &lParam);

		void Release()
		{
			delete this;
		}
		CProc()
		{
		}

	};
	CProc* CreateProc(LPCTSTR szProc);
	int ExecProc(CProc* pProc);

};

