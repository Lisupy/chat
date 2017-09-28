#pragma once
#include<WinSock2.h>
#include "Peer.h"

class Server: public Peer{
protected:
	Server(int port);
	int Bind();
	int Listen();
public:
	// create a new Server by port
	static Server* newServer(int port);
	
	Peer waitClient();
	~Server()=default;
};

