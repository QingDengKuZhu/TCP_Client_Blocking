#ifndef MY_FUNCTION_H
#define MY_FUNCTION_H

#include <WinSock2.h>
#include <iostream>

#define  BUFFER_SIZE		SO_MAX_MSG_SIZE	//缓冲区大小
#define  DEF_PORT			10000

//将用户在命令行似乎入的地址转换成整数地址.用户输入的地址可以有两种形式:点分十进制(如 192.0.0.1)和主句名称形式(如localhost).
//szServer: 待处理字符串.
//若成功,返回表示的服务器地址(主句字节序);失败返回INADDR_NONE.
u_long ResoleAddress(const char* szServer);

//客户端主体函数.
//szServer: 待处理的字符串.
//nPort: 合法端口号.
void DoWork(const char* szServer, int nPort);

//连接服务器.
//nServerAddr: 合法的服务器地址.
//nPort: 合法的端口号.
//若成功,返回连接句柄 否者,返回INVALID_SOCKET.
SOCKET ConnectServer(u_long nServerAddr, int nPort);

//处理回显业务逻辑.
//sd:已连接的套接字句柄.
//若成功,返回true;否者返回false.
bool ProcessConnection(SOCKET sd);

//关闭连接.
//sd: 已连接的套接字句柄.
//若成功,返回true;否者返回false.
bool ShutdownConnection(SOCKET sd);

//完整发送数据.
//sd: 已连接套接字
//data: 待发送数据的地址
//len: 数据大小
//若成功发送,返回true;否者返回false.
bool CompleteSend(SOCKET sd, const char *data, int len);

//完整接收数据.
//sd : 已连接套接字
//buffer : 接收数据的缓冲区地址
//len :"数据大小
//若成功接收,返回true;否者返回false.
bool CompleteRecv(SOCKET sd, char *buffer, int len);

#endif