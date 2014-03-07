#include <iostream>
#include "my_function.h"

#pragma comment(lib, "ws2_32.lib")

using std::endl;
using std::cout;
using std::cin;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		return EXIT_FAILURE;
	}
	
	const char* szServer = argv[1];
	
	int nPort = DEF_PORT;
	if (argc >3)
	{
		nPort = atoi(argv[2]);
	}

	WSADATA wsadata;
	if (SOCKET_ERROR == WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		cout << "WSAStartup error : " << WSAGetLastError() << endl;
		return EXIT_FAILURE;
	}


	DoWork(szServer, nPort);

	if (SOCKET_ERROR == WSACleanup())
	{
		cout << "WSAClearup error : " << WSAGetLastError() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;


}