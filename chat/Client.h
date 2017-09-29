#pragma once
#include "Peer.h"
class Client:public Peer
{
public:
	Client(const char *ip, int port):Peer() {
		_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		std::memset(&_address, 0, sizeof(sockaddr_in));
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = inet_addr(ip);
		_address.sin_port = htons(port);
	}
	~Client() {}
	int Connect() {
		return connect(_sock, (SOCKADDR*)&_address, sizeof(SOCKADDR));
	}
};

