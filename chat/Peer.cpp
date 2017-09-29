#include "Peer.h"
#include <errno.h>
#include <cstring>

std::string recvMessage(const Peer &p) {
	std::string msg;

	int msgLen = 0;
	int n = recv(p._sock, (char*)&msgLen, sizeof(msgLen), 0);
	if (n <= 0)
		return msg;

	char buff[BUFFLEN];
	std::memset(buff, 0, sizeof(buff));
	while (msgLen > 0) {
		int recv_len = msgLen > BUFFLEN-1 ? BUFFLEN-1 : msgLen;
		int n = recv(p._sock, buff, recv_len, 0);
		msgLen -= n;
		buff[recv_len] = '\0';
		msg += buff;
	}
	return msg;
}

int sendMessage(const Peer &p, const std::string &s) {
	int len = s.length();
	send(p._sock, (char*)&len, sizeof(len), 0);
	int n = send(p._sock, s.c_str(), len, 0);
	return n;
}