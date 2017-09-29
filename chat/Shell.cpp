#include "Shell.h"
#include "Server.h"
#include "Client.h"
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
	fun_mapper["list"] = &Shell::_list;
	fun_mapper["exit"] = &Shell::_exit;

	serv = Server::newServer(port);
}

/* friend function of shell to accept client request*/
void start_server(Shell *shell) {
	std::cout << "server started..." << std::endl;
	while (true) {
		Peer clnt = shell->serv->waitClient();

		shell->_screen_mtx.lock();
		std::cout << "Peer "
			<< clnt.get_ip() << ":" << clnt.get_port()
			<< " connected to server" << std::endl;
		shell->_screen_mtx.unlock();

		shell->_vec_mtx.lock();
		std::thread(recv_client, std::ref(clnt), shell).detach();
		shell->_clnt_vec.push_back(clnt);
		shell->_vec_mtx.unlock();
	}
}

void recv_client(Peer &clnt, Shell *shell) {
	std::string msg;
	while (true) {
		msg = recvMessage(clnt);
		if (msg.length() == 0)
			break;
		shell->_screen_mtx.lock();
		std::cout << "Message received from " << clnt.get_ip()
			<< "\nSender's port: " << clnt.get_port()
			<< "\nMessage: \"" << msg << "\"" << std::endl;
		shell->_screen_mtx.unlock();
	}

	shell->_vec_mtx.lock();
	auto beg = shell->_clnt_vec.begin();
	while (beg->get_sock() != clnt.get_sock()) ++beg;
	shell->_clnt_vec.erase(beg);
	shell->_vec_mtx.unlock();
	closesocket(clnt.get_sock());

	shell->_vec_mtx.lock();
	std::cout << clnt.get_ip() << ":" << clnt.get_port() << " disconnect." << std::endl;
}

void Shell::start() {
	serv_th = std::move(std::thread(start_server, this));
	std::string command;
	while (true) {
		std::cin >> command;
		auto fun = fun_mapper.find(command);
		if (fun == fun_mapper.end()) {
			_screen_mtx.lock();
			std::cout << "command not exists, using help command for some help" << std::endl;
			_screen_mtx.unlock();
			continue;
		}
		void( Shell::*f)() = fun->second;
		(this->*f)();

		_screen_mtx.lock();
		std::cout << "-------------------------------" << std::endl;
		_screen_mtx.unlock();
	}
}

void Shell::_help() {
	_screen_mtx.lock();
	std::cout << "The following command is valid:\n\n"
		<< "help:\tGet help from shell\n"
		<< "myip:\tWhow ip address of your computers\n"
		<< "myport:\tShow the port your server is listening to\n"
		<< "connect:\tEstablish new connection to specified computer. connect <ip> <port>\n"
		<< "list:\tDisplay a numbered list of all the connections this process is part of\n"
		<< "terminate:\tterminate the connection showed on list. terminate <id>\n"
		<< "send:\tsend message to the connection showed by list command. send <id> <msg>\n"
		<< "exit:\tclose all connection and exit"
		<< std::endl;
	_screen_mtx.unlock();
}

void Shell::_myip() {
	int nLen = 256;
	char hostname[20];
	gethostname(hostname, nLen);
	hostent *pHost = gethostbyname(hostname);
	LPSTR lpAddr = pHost->h_addr_list[0];
	in_addr inetAddr;
	memmove(&inetAddr, lpAddr, 4);
	_screen_mtx.lock();
	std::cout << "Your IP address: " << inet_ntoa(inetAddr) << std::endl;
	_screen_mtx.unlock();
}

void Shell::_myport() {
	_screen_mtx.lock();
	std::cout << "Your server port is " << serv->get_port() << std::endl;
	_screen_mtx.unlock();
}

void Shell::_connect() {
	std::string destination;
	int port;
	std::cin >> destination >> port;

	Client *clnt = new Client(destination.c_str(), port);
	if (clnt->Connect() < 0) {
		_screen_mtx.lock();
		std::cout << "Can not connect to " << destination << " " << port << std::endl;
		_screen_mtx.unlock();
		return;
	}

	_vec_mtx.lock();
	std::thread(recv_client, std::ref(*clnt), this).detach();
	_clnt_vec.push_back(*clnt);
	_vec_mtx.unlock();

	_screen_mtx.lock();
	std::cout << "Connect to " << destination << ":" << port << " OK" << std::endl;
	_screen_mtx.unlock();
}

void Shell::_list() {
	std::try_lock(_vec_mtx, _screen_mtx);
	std::cout << "id:    IP address      Port No." << std::endl;
	std::cout << "===============================" << std::endl;
	int i = 1;
	for (auto it = _clnt_vec.begin(); it != _clnt_vec.end(); ++it) {
		std::cout << i++ << ":    " << it->get_ip()
			<< "      " << it->get_port() << std::endl;
	}
	_vec_mtx.unlock();
	_screen_mtx.unlock();
}

void Shell::_terminate() {
	int id;
	std::cin >> id;
	std::try_lock(_vec_mtx, _screen_mtx);
	if (id <= _clnt_vec.size() && id > 0) {
		auto it = _clnt_vec.begin() + id - 1;
		closesocket(it->get_sock());
		std::cout << "Success" << std::endl;
	}
	_vec_mtx.unlock();
	_screen_mtx.unlock();
}

void Shell::_send() {
	int id;
	std::string msg;
	std::cin >> id;
	std::getline(std::cin, msg);
	sendMessage(_clnt_vec[id - 1], msg);
}

void Shell::_exit() {
	for (const auto& clnt : _clnt_vec) {
		closesocket(clnt.get_sock());
	}
	closesocket(serv->get_sock());
	exit(0);
}