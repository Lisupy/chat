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

	std::string get_ip() const { return inet_ntoa(_address.sin_addr); }
	short get_port() const { return ntohs(_address.sin_port); }
	SOCKET get_sock()const { return _sock; }

	friend std::string recvMessage(const Peer &p);
	friend int sendMessage(const Peer &p, const std::string &s);
};