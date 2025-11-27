// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


// ChatServer-Application.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "boost/beast/core.hpp"
#include "boost/beast.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio.hpp" 
#include "boost/asio/ip/tcp.hpp"
#include "Header/Server.hpp"

//main entry point
int main(int16_t argc , int8_t **argv)
{
	try {

		i_asio::io_context ctx; 
		i_tcp::endpoint port = { i_asio::ip::make_address("127.0.0.1") , 8080};
		i_Server::Server server(ctx, port);
		//server.Server_get_the_config(); 


		server.Server_run();
		return 0;
	}
	catch (const std::exception& exc) {
		std::cerr << "The error has been caugth - " << exc.what() << std::endl;
		throw;
	}
}
