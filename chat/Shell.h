#pragma once

#include"Peer.h"
#include"Server.h"
#include<mutex>
#include<thread>
#include<map>
#include<vector>

class Shell{
protected:
	void _help();
	void _myip();
	void _myport();
	void _connect();
	void _list();
	void _terminate();
	void _send();
	void _exit();

public:
	
	Shell(int port);
	~Shell() = default;
	void start();

	friend void start_server(Shell *);

private:
	std::vector<std::thread> thread_vector; // thread vector
	std::vector<Peer> peer_vector;          // peer vector

	std::mutex _screen_mtx;  // mutex for print message on screen
	std::mutex _peer_mtx;    // mutex for peer vector

	std::vector<Peer> _peers;  // peer information
	std::map<std::string, void (Shell::*)()> fun_mapper;
	Server *serv;
};

