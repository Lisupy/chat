#include<iostream>
#include<cstring>
#include<WinSock2.h>
#include<thread>
#include<mutex>
#include<utility>
#include "Shell.h"
#include "Client.h"

#pragma comment(lib, "ws2_32.lib")  // load ws2_32.dll

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "usage: .\\chat <port_num>" << std::endl;
		return 1;
	}

	// initialize dll
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Winsock initialize failed" << std::endl;
		return 2;
	}

	Shell shell(atoi(argv[1]));
	shell.start();

	// stop using dll
	WSACleanup();

	return 0;
}