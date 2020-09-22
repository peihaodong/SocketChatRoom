//#include "StdAfx.h"
#include "PhdSocket.h"
#include <ws2tcpip.h>
#include <clocale>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#pragma comment (lib,"ws2_32.lib")
#endif

PhdSocket::PhdSocket()
{
#ifdef _WIN32
	WSAData wd;
	WSAStartup(0x0202, &wd);
#endif
	m_hSocket = INVALID_SOCKET;
}

PhdSocket::~PhdSocket()
{
	std::cout << m_hSocket << " socket被析构" << std::endl;
	this->Close();
}

void PhdSocket::Close()
{
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

bool PhdSocket::IsNull() const
{
	if (m_hSocket == INVALID_SOCKET)
		return true;
	else
		return false;
}

BOOL PhdSocket::Create(UINT nSocketPort /*= 0*/, 
	int nSocketType /*= SOCK_STREAM */, LPCTSTR lpszSocketAddress /*= NULL*/)
{
	//创建socket
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}
	//创建地址信息结构体：IP和端口
	sockaddr_in sa = { AF_INET,htons(nSocketPort) };
	if (lpszSocketAddress)
	{
		//IP地址传入到地址结构体中（InetPton用于宽字节，inet_pton用于多字节）
		Pton(lpszSocketAddress, sa);
	}
	//socket绑定地址结构体
	return !bind(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

BOOL PhdSocket::Listen(int nConnectionBacklog /*= 5*/) const
{
	return !listen(m_hSocket, nConnectionBacklog);
}

BOOL PhdSocket::Accept(PhdSocket& rConnectedSocket, std::string& strIP, UINT *nPort) const
{
	//创建服务地址结构体
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	rConnectedSocket.m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == rConnectedSocket.m_hSocket)
		return FALSE;

	//得到ip地址
	Ntop(sa, strIP);

	//得到端口号
	*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL PhdSocket::Accept(PhdSocket& rConnectedSocket, std::wstring& strIP, UINT *nPort) const
{
	//创建服务地址结构体
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	rConnectedSocket.m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == rConnectedSocket.m_hSocket)
		return FALSE;

	//得到ip地址
	Ntop(sa, strIP);

	//得到端口号
	*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL PhdSocket::Accept(PhdSocket* pSocket, std::string& strIP, UINT *nPort) const
{
	//创建服务地址结构体
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	pSocket->m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == pSocket->m_hSocket)
		return FALSE;

	//得到ip地址
	Ntop(sa, strIP);

	//得到端口号
	*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL PhdSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort) const
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	Pton(lpszHostAddress, sa);
	return !connect(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

int PhdSocket::Send(const void* lpBuf, int nBufLen, int nFlags /*= 0*/) const
{
	return send(m_hSocket, (const char*)lpBuf, nBufLen, nFlags);
}

int PhdSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress /*= NULL*/) const
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	Pton(lpszHostAddress, sa);
	sendto(m_hSocket, (const char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, sizeof(sa));
	return 0;
}

int PhdSocket::Receive(void* lpBuf, int nBufLen, int nFlags /*= 0*/) const
{
	return recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
}

int PhdSocket::ReceiveFrom(void* lpBuf, int nBufLen, std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet <= 0)
		return nRet;

	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);

	return nRet;
}

int PhdSocket::ReceiveFrom(void* lpBuf, int nBufLen, std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet <= 0)
		return nRet;

	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);

	return nRet;
}

BOOL PhdSocket::GetPeerName(std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getpeername(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetPeerName(std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getpeername(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetSockName(std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getsockname(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetSockName(std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getsockname(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

void PhdSocket::Ntop(const sockaddr_in& sa, std::wstring& strIP) const
{
#ifdef _UNICODE
	wchar_t wcIP[INET_ADDRSTRLEN];
	InetNtop(AF_INET, (void *)&sa.sin_addr, wcIP, sizeof(wcIP));
	strIP.assign(wcIP);
#else
	char cIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (void *)&sa.sin_addr, cIP, sizeof(cIP));
	wchar_t* wcIP = CToW(cIP);
	strIP.assign(wcIP);
	delete[] wcIP;
#endif	
}

void PhdSocket::Ntop(const sockaddr_in& sa, std::string& strIP) const
{
#ifdef _UNICODE
	wchar_t wcIP[INET_ADDRSTRLEN];
	InetNtop(AF_INET, (void *)&sa.sin_addr, wcIP, sizeof(wcIP));
	char* cIP = WToC(wcIP);
	strIP.assign(cIP);
	delete[] cIP;
#else
	char cIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (void *)&sa.sin_addr, cIP, sizeof(cIP));
	strIP.assign(cIP);
#endif
}

wchar_t* PhdSocket::CToW(char* str) const
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = std::strlen(str);

	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP, 0, str, charLen, NULL, 0);

	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	wchar_t* buf = new wchar_t[len + 1];

	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, str, charLen, buf, len);

	buf[len] = '\0'; //添加字符串结尾，注意不是len+1

	return buf;
}

std::wstring PhdSocket::CToW(const std::string& str) const
{
	size_t i;
	std::string curLocale = std::setlocale(LC_ALL, NULL);
	std::setlocale(LC_ALL, "chs");
	const char* _source = str.c_str();
	size_t _dsize = str.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	std::wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;
	std::setlocale(LC_ALL, curLocale.c_str());
	return result;
}

char* PhdSocket::WToC(wchar_t* str) const
{
	//计算wchar_t *数组大小，以字节为单位，一个汉字占两个字节
	int wcharLen = std::wcslen(str);

	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, wcharLen, NULL, 0, NULL, NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char* buf = new char[len + 1]; //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, wcharLen, buf, len, NULL, NULL);

	buf[len] = '\0'; //多字节字符以'\0'结束

	return buf;
}

std::string PhdSocket::WToC(const std::wstring& str) const
{
	size_t i;
	std::string curLocale = std::setlocale(LC_ALL, NULL);
	std::setlocale(LC_ALL, "chs");
	const wchar_t* _source = str.c_str();
	size_t _dsize = 2 * str.size() + 1;
	char* _dest = new char[_dsize];
	std::memset(_dest, 0x0, _dsize);
	wcstombs_s(&i, _dest, _dsize, _source, _dsize);
	std::string result = _dest;
	delete[] _dest;
	std::setlocale(LC_ALL, curLocale.c_str());
	return result;
}

