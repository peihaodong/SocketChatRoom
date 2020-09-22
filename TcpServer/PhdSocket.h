#pragma once
/*
1、   WIN32/_WIN32 可以用来判断是否 Windows 系统（对于跨平台程序）
2、 _WIN64 用来判断编译环境是 x86（32位） 还是 x64（64位）
		在 Win32 配置下，_WIN32 有定义，_WIN64 没有定义。
		 在 x64 配置下，_WIN32 和_WIN64 都有定义。
*/

#include <stdlib.h>
#include <cwchar>
#include <cstring>
#include <ws2tcpip.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <process.h>
#ifdef _UNICODE
#include <comdef.h>
//#include <WS2tcpip.h>
#endif
typedef int socklen_t;
typedef void RET_TYPE;
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
typedef unsigned int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#ifndef _T
#define _T(x) x
#endif
typedef struct in_addr IN_ADDR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL            *PBOOL;
typedef BOOL             *LPBOOL;
typedef BYTE            *PBYTE;
typedef BYTE             *LPBYTE;
typedef int             *PINT;
typedef int              *LPINT;
typedef WORD            *PWORD;
typedef WORD             *LPWORD;
typedef long             *LPLONG;
typedef DWORD           *PDWORD;
typedef DWORD            *LPDWORD;
typedef void             *LPVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef const char* LPCTSTR, *LPCSTR;
typedef char* LPTSTR, *LPSTR;
typedef void* RET_TYPE;
inline int GetLastError()
{
	return errno;
}
#define closesocket(x) close(x)


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
#endif
#include <string>

class PhdSocket
{
public:
	PhdSocket();
	virtual ~PhdSocket();

	operator SOCKET() const{
		return m_hSocket;
	}

	bool operator<(const PhdSocket& obj)const {
		return m_hSocket < obj.m_hSocket;
	}

public:
	//关闭socket
	void Close();

	// Summary:   该对象是否为空
	// Time:	  2020年3月31日 peihaodong
	// Explain:	  
	bool IsNull() const;

	//创建socket并绑定
	//nSocketPort - 端口；如果是0，绑定随机端口
	//lpszSocketAddress - IP地址
	BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM
		, LPCTSTR lpszSocketAddress = NULL);

	// Summary:   侦听
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  nConnectionBacklog - 代表同时能连接5个客户端，超过5个需要等待
	BOOL Listen(int nConnectionBacklog = 5) const;

	// Summary:   接收（阻塞操作，有客户端连接会生成新的PhdSocket对象）
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  返回PhdSocket对象的IP和端口
	BOOL Accept(PhdSocket& rConnectedSocket,std::string& strIP,
		UINT *nPort) const;
	BOOL Accept(PhdSocket* pSocket, std::string& strIP,
		UINT *nPort) const;
	BOOL Accept(PhdSocket& rConnectedSocket, std::wstring& strIP,
		UINT *nPort) const;

	// Summary:   连接服务器（通过服务器前台IP和端口）
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort) const;

	//************************************
	// Summary:  发送数据，向连接的socket
	// Parameter:
	//	  lpBuf -	要发送的数据指针（首地址）
	//	  nBufLen -	要发送的数据数量（字符串的数量）
	//	  nFlags -	
	// Return:	 返回发送的字符数量
	// Notice:   
	// Update Time: 2020-9-8 13:48:45
	//************************************
	int Send(const void* lpBuf, int nBufLen, int nFlags = 0) const;
	// Summary:   发送数据 向该地址结构体 
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  返回发送的字符数量
	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
		LPCTSTR lpszHostAddress = NULL) const;	

	//************************************
	// Summary:  接收数据，向连接的socket
	// Parameter:
	//	  lpBuf -	new的缓冲区指针
	//	  nBufLen -	缓冲区的大小
	//	  nFlags -	
	// Return:	 返回接收的字符数量
	// Notice:   
	// Update Time: 2020-9-8 13:53:03
	//************************************
	int Receive(void* lpBuf, int nBufLen, int nFlags = 0) const;
	// Summary:   接收数据 得到发送端的IP和端口
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  返回接收的字符数量
	int ReceiveFrom(void* lpBuf, int nBufLen,
		std::string& strIP, UINT& rSocketPort) const;
	int ReceiveFrom(void* lpBuf, int nBufLen,
		std::wstring& strIP, UINT& rSocketPort) const;

	// Summary:   得到socket信息
	// Time:	  2020年3月30日 peihaodong
	// Explain:	  
	BOOL GetPeerName(std::string& strIP, UINT& rSocketPort) const;
	BOOL GetPeerName(std::wstring& strIP, UINT& rSocketPort) const;
	
	//************************************
	// Summary:  获取本地的socket信息（ip地址得不到）
	// Update Time: 2020-9-9 10:32:25
	//************************************
	BOOL GetSockName(std::string& strIP, UINT& rSocketPort) const;
	BOOL GetSockName(std::wstring& strIP, UINT& rSocketPort) const;

private:
	//通过地址结构体得到IP地址
	void Ntop(const sockaddr_in& sa, std::wstring& strIP) const;
	void Ntop(const sockaddr_in& sa, std::string& strIP) const;

#ifdef _UNICODE
	//IP地址传入到地址结构体中
	inline void Pton(LPCTSTR sHostAddr, sockaddr_in& sa) const {
		InetPton(AF_INET, sHostAddr, &sa.sin_addr.s_addr);
	}
#else
	//IP地址传入到地址结构体中
	inline void Pton(LPCTSTR sHostAddr, sockaddr_in& sa) const {
		inet_pton(AF_INET, sHostAddr, &sa.sin_addr.s_addr);
	}
#endif

	//************************************
	// Summary:  多字节字符串转宽字节字符串
	// Parameter:
	//	  str -	
	// Explain:	 需要手动delete
	// Time:	  2020年9月4日 peihaodong
	//************************************
	wchar_t* CToW(char* str) const;
	std::wstring CToW(const std::string& str) const;
	//************************************
	// Summary:  宽字节字符串转多字节字符串
	// Parameter:
	//	  str -	
	// Explain:	 需要手动delete
	// Time:	  2020年9月4日 peihaodong
	//************************************
	char* WToC(wchar_t* str) const;
	std::string WToC(const std::wstring& str) const;
	

private:
	SOCKET m_hSocket;//核心成员变量
};