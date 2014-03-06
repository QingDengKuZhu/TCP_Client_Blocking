#ifndef MY_FUNCTION_H
#define MY_FUNCTION_H

#include <WinSock2.h>
#include <iostream>

#define  BUFFER_SIZE		1024	//缓冲区大小

//将用户在命令行似乎入的地址转换成整数地址.用户输入的地址可以有两种形式:点分十进制(如 192.0.0.1)和主句名称形式(如localhost).
//若成功,返回地址;失败返回INADDR_NONE.
u_long ResoleAddress(const char* szServer);

//客户端主体函数

void DoWork(const char* szServer, int nPort);

//连接服务器.
//若成功,返回连接
SOCKET ConnectServer(u_long ServerAddr, int nPort);

//处理回显业务逻辑
//若成功,返回true;否者返回false
bool ProcessConnection(SOCKET sd);

//关闭连接
//若成功,返回true;否者返回false.
bool ShutdownConnection(SOCKET sd);

#endif