#include "Shell.h"
#include "Server.h"
#include <thread>
#include <utility>
#include <iostream>


Shell::Shell(int port) {
	fun_mapper["help"] = &Shell::_help;
	fun_mapper["myip"] = &Shell::_myip;
	fun_mapper["myport"] = &Shell::_myport;
	fun_mapper["connect"] = &Shell::_connect;
	fun_mapper["terminate"] = &Shell::_terminate;
	fun_mapper["send"] = &Shell::_send;

	Server *serv = Server::newServer(port);
	thread_vector.push_back(std::thread(start_server, this));
}

/* friend function of shell to accept client request*/
void start_server(Shell *shell) {
	std::cout << "server started..." << std::endl;
	while (true) {
		Peer clnt = shell->serv->waitClient();
		shell->_peer_mtx.lock();
		shell->_peers.push_back(clnt);
		shell->_peer_mtx.unlock();
	}
}

void Shell::start() {
	std::string command;
	while (true) {
		std::cin >> command;
		if (command == "exit")
			break;
		auto fun = fun_mapper.find(command);
		if (fun == fun_mapper.end()) {
			_screen_mtx.lock();
			std::cout << "command not exists, using help command for some help" << std::endl;
			_screen_mtx.unlock();
			continue;
		}
		void( Shell::*f)() = fun->second;
		(this->*f)();
	}
}

void Shell::_help() {

}

void Shell::_myip() {

}

void Shell::_myport() {

}

void Shell::_connect() {

}

void Shell::_list() {

}

void Shell::_terminate() {

}

void Shell::_send() {

}

void Shell::_exit() {

}