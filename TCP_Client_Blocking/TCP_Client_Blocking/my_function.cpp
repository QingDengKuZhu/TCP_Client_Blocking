#include <WinSock2.h>
#include <iostream>
#include "my_function.h"

using std::endl;
using std::cout;
using std::cin;

u_long ResoleAddress(const char* szServer)
{
	//首先尝试使用inet_addr来转换点分十进制地址,如果返回的是INADDR_NONE.
	//则表明szServer使用的是主机名称形式,此时需要调用gethostbyname来查询起地址

	u_long nAddr = inet_addr(szServer);
	if (INADDR_NONE == nAddr)
	{
		hostent *ent = gethostbyname(szServer);
		if (ent == NULL)
		{
			cout << "gethostbyname error : " << WSAGetLastError() <<endl; 
		}
		else
		{
			nAddr = *((u_long *)ent->h_addr_list[0]);
		}
	}
	
	if (INADDR_NONE == nAddr)
	{
		cout << "Error resolving address !" << endl; 
	}

	return ntohl(nAddr);	//必须转换成主机字节序!!!
}

void DoWork(const char* szServer, int nPort)
{
	//处理地址数据
	u_long nServerAddr = ResoleAddress(szServer);
	if (INADDR_NONE == nServerAddr)
	{
		return;
	}
	
	//第一阶段, 连接服务器
	SOCKET sd = ConnectServer(nServerAddr, nPort);
	if (INVALID_SOCKET == sd)
	{
		cout << "ConnectServer error : " << WSAGetLastError() << endl;
		return;
	}

	//第二阶段,处理回显业务逻辑
	if (true == ProcessConnection(sd))
	{
		//第三阶段,关闭连接.
		ShutdownConnection(sd);
	}

	return;
}

SOCKET ConnectServer(u_long nServerAddr, int nPort)
{
	SOCKET sd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sd)
	{
		cout << "socket error : " << WSAGetLastError() << endl;
		return INVALID_SOCKET;
	}
	
	sockaddr_in AddrServer = {0};
	//填充服务器端套接字地址
	AddrServer.sin_family = AF_INET;
	AddrServer.sin_addr.S_un.S_addr = htonl(nServerAddr);
	AddrServer.sin_port = htons(nPort);
	
	//连接服务器
	if ( SOCKET_ERROR == connect(sd, (const sockaddr *)&AddrServer, sizeof(sockaddr)) )
	{
		cout << "connect error : " << WSAGetLastError() << endl;
		closesocket(sd);

		return INVALID_SOCKET; 
	}

	return sd;
}

bool CompleteSend(SOCKET s, const char *data, int len)
{
	int idex = 0;
	char *p = (char *)data;

	while (idex < len)
	{
		int nTemp = send(s, p+idex, len-idex, 0);
		if (nTemp > 0)
		{
			idex += nTemp;
		}
		else if (nTemp == SOCKET_ERROR)
		{
			cout << "send error : " << WSAGetLastError() << endl;
		}
		else	//nTemp==0表示连接关闭
		{
			cout << "Connection closed unexpectedly by peer." << endl;
			return true;
		}
	}

	return true;	//当传入的len为0时,将执行此语句
}

bool CompleteRecv(SOCKET s, char *buffer, int len)
{
	int idex = 0;
	while (idex < len)
	{
		int nTemp = recv(s, buffer, len-idex, 0);
		if (nTemp == SOCKET_ERROR)
		{
			cout << "recv error : " <<WSAGetLastError() << endl;
			
			return false;
		}
		else if (nTemp > 0)
		{
			idex += nTemp;
		}
		else
		{
			cout << "Connection closed unexpectedly by peer." << endl;
			
			return true;
		}
	}
	//当传入的len为0时,将执行此语句
	
	return true;
}

bool ProcessConnection(SOCKET sd)
{
	char buff[BUFFER_SIZE] = {0};
	cout << "Please input data : " << endl; 
	//直到用户输入Ctrl+Z为止
	while (cin>>buff)
	{
		if (false == CompleteSend(sd, buff, (int)strlen(buff)))
		{
			return false;
		}

		if (false == CompleteRecv(sd, buff, (int)strlen(buff)))
		{
			return false;
		}

		buff[(int)strlen(buff)] = '\0';
		cout << buff << endl;
	}

	return true;
}

bool ShutdownConnection(SOCKET sd)
{
	if (SOCKET_ERROR == shutdown(sd, SD_SEND))
	{
		cout << "shutdown error : " << WSAGetLastError() << endl;
		return false;
	}
	
	char buff[BUFFER_SIZE] = {0};
	int result = 0;
	
	do 
	{
		result = recv(sd, buff, BUFFER_SIZE, 0);
		if (SOCKET_ERROR == result)
		{
			cout << "recv error : " << WSAGetLastError() << endl;
			return false;
		}
		else if (result > 0)
		{
			cout << result << "unexpected bytes received." << endl;
		}
	} while (0 != result);

	if (SOCKET_ERROR == closesocket(sd))
	{
			cout << "closesocket error : " << WSAGetLastError() << endl;
			return false;
	}

	return true;
}
