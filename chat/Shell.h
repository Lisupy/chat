#pragma once

#include"Peer.h"
#include"Server.h"
#include<mutex>
#include<thread>
#include<map>
#include<vector>
#include<utility>

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
	friend void recv_client(Peer &clnt, Shell *);

private:

	std::vector<Peer> _clnt_vec;

	std::mutex _screen_mtx;  // mutex for print message on screen
	std::mutex _vec_mtx;    // mutex for peer vector

	std::map<std::string, void (Shell::*)()> fun_mapper;
	Server *serv;
	std::thread serv_th;
};

