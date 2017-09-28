#include "Peer.h"
#include "Server.h"
#include <iostream>
#include <WinSock2.h>
#include <cstring>

Server::Server(int port){
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	std::memset(&_address, 0, sizeof(sockaddr_in));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(port);
}

int Server::Bind() {
	return bind(_sock, reinterpret_cast<SOCKADDR*>(&_address), sizeof(_address));
}

int Server::Listen() {
	return listen(_sock, 32);
}

Server* Server::newServer(int port) {
	if (port <= 1024)
		return nullptr;
	
	Server* serv_p = new Server(port);
	if (serv_p->Bind() != 0) {
		delete serv_p;
		std::cerr << "Failed to bind on"<< port << std::endl;
		return nullptr;
	}
	if (serv_p->Listen() != 0) {
		delete serv_p;
		std::cerr << "Failed to listen on" << port << std::endl;
		return nullptr;
	}
	return serv_p;
}

Peer Server::waitClient() {
	sockaddr_in clntAddress;
	int addrlen = sizeof(clntAddress);
	
	// receive connection from client
	SOCKET clntSock = accept(_sock, reinterpret_cast<SOCKADDR*>(&clntAddress), &addrlen);
	
	return Peer(clntSock, clntAddress);
}