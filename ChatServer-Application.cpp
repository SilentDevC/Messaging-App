// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
// ChatServer-Application.cpp : This file contains the 'main' function. Program execution begins and ends there.
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Targets Windows 10/11
#endif
#include <iostream>
#include "boost/asio/ip/tcp.hpp"
#include "Header/Server.hpp"

//main entry point
int main(int16_t argc , char **argv)
{
	try {
		//i_asio::io_context ctx; 
		//i_tcp::endpoint port = { i_asio::ip::make_address("127.0.0.1") , 8080};
		//
		////server.Server_get_the_config(); 

		//i_Server::Server server(ctx, port , fin_db);

		//server.Server_run();
		//return 0;

		asio::io_context ctx; 

		DB_worker::dbworker worker(ctx, "localhost", 3306, "root", "admin");
		

		std::string query = "SELECT * FROM USERS";
		auto res_future = worker.dbworker_addjobs(query);
		std::cout << res_future.get().size() << std::endl;
	}
	catch (const std::exception& exc) {
		std::cerr << "The error has been caugth - " << exc.what() << std::endl;
		throw;
	}
}
