#include "Peer.h"

std::string recvMessage(const Peer &p) {
	std::string msg;

	int msgLen = 0;
	recv(p._sock, (char*)&msgLen, sizeof(msgLen), 0);

	char buff[BUFFLEN];
	while (msgLen > 0) {
		int recv_len = msgLen > BUFFLEN ? BUFFLEN : msgLen;
		int n = recv(p._sock, buff, recv_len, 0);
		msgLen -= n;
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