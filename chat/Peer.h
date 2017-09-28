#pragma once
#include <WinSock2.h>
#include <string>

const unsigned BUFFLEN = 4096;

class Peer{
protected:
	SOCKET _sock;
	sockaddr_in _address;

public:

	Peer() = default;
	Peer(SOCKET sock, sockaddr_in address) : _sock(sock), _address(address) {}
	~Peer() = default;
	friend std::string recvMessage(const Peer &p);
	friend int sendMessage(const Peer &p, const std::string &s);
};