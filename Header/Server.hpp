// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
// //Server definition header
//---------//
#ifndef CXX_CHAT_SERVER_DEF_HPP
#define CXX_CHAT_SERVER_DEF_HPP
#include <optional>
#include <string>
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include "../Source/Utils.cpp"
#include "../Source/MySQL_DB_Connect.cpp"
#include "Header/Concurency.hpp"

//-----------//
namespace server_routing {
	using HRoute = std::function<void(std::shared_ptr<Session>)>;
	//------------//
	const inline std::shared_ptr<std::unordered_map<std::string, HRoute>> hRoutes =
		std::make_shared<std::unordered_map<std::string, HRoute>>(
			std::initializer_list<std::pair<const std::string, HRoute>>{
				{ "login", HRoute() },
				{ "users",  HRoute() },
				{ "passwords", HRoute() }
			}
		);
}
//------------// 
namespace http_request_handle {
	//---------------------//

	//---------------------//
	void http_GET_request_response(std::shared_ptr<Session> session_,
								   const std::string& msg,
								   const i_http::status& stat = i_http::status::ok) {
		// Prepare the response message
		i_http::response<i_http::string_body> response;

		response.version(session_->request.version());
		response.result(stat);
		response.set(i_http::field::server, "Chat_Server");
		response.set(i_http::field::content_type, "text/plain");
		response.set(i_http::field::date, get_current_http_date());
		response.body() = msg;
		response.prepare_payload();
		session_->response = response;
		// Send the response to the client
		boost::beast::http::async_write(session_->socket, session_->response,
			[session_](boost::system::error_code ec, size_t bytes_transferred) {
				if (ec) {
					std::cerr << "The error has occured using boost : " << ec.message() << ", trasferred :: " << bytes_transferred << " bytes!" << std::endl;
				}
			});
	}
	//template <typename _ty>
	void http_POST_request_response(std::shared_ptr<Session> session_,
									const std::string& msg, 
									const i_http::status& stat = i_http::status::ok
									/*const _ty& up_value_*/ ) {



		i_http::response<i_http::string_body> response;
		response.version(session_->request.version());
		response.version(session_->request.version());
		response.result(stat);
		response.set(i_http::field::server, "Chat_Server");
		response.set(i_http::field::content_type, "text/plain");
		response.set(i_http::field::date, get_current_http_date());
		response.body() = msg;
		response.prepare_payload();
		session_->response = response;
		// Send the response to the client
		boost::beast::http::async_write(session_->socket, session_->response,
			[session_](boost::system::error_code ec, size_t bytes_transferred) {
				if (ec) {
					std::cerr << "The error has occured using boost : " << ec.message() << ", trasferred :: " << bytes_transferred << " bytes!" << std::endl;
				}
			});

	}
	void http_DELETE_request_response(std::shared_ptr<Session> session_,
									  const std::string& msg,
									  const i_http::status& stat = i_http::status::ok) {
		//process the delete 

		i_http::response<i_http::string_body> response;
		response.version(session_->request.version());
		response.version(session_->request.version());
		response.result(stat);
		response.set(i_http::field::server, "Chat_Server");
		response.set(i_http::field::content_type, "text/plain");
		response.set(i_http::field::date, get_current_http_date());
		response.body() = msg;
		response.prepare_payload();
		session_->response = response;

		// Send the response to the client
		boost::beast::http::async_write(session_->socket, session_->response,
			[session_](boost::system::error_code ec, size_t bytes_transferred) {
				if (ec) {
					std::cerr << "The error has occured using boost : " << ec.message() << ", trasferred :: " << bytes_transferred << " bytes!" << std::endl;
				}
			});

	}
	void http_PATCH_request_response(std::shared_ptr<Session> session_,
									  const std::string& msg,
									  const i_http::status& stat = i_http::status::ok) {
		//process the delete 

		i_http::response<i_http::string_body> response;
		response.version(session_->request.version());
		response.version(session_->request.version());
		response.result(stat);
		response.set(i_http::field::server, "Chat_Server");
		response.set(i_http::field::content_type, "text/plain");
		response.set(i_http::field::date, get_current_http_date());
		response.body() = msg;
		response.prepare_payload();
		session_->response = response;

		// Send the response to the client
		boost::beast::http::async_write(session_->socket, session_->response,
			[session_](boost::system::error_code ec, size_t bytes_transferred) {
				if (ec) {
					std::cerr << "The error has occured using boost : " << ec.message() << ", trasferred :: " << bytes_transferred << " bytes!" << std::endl;
				}
			});
	}
}
//---------//
//internal Server namespace to isolate from potential errors 
namespace i_Server {
	//---------//
	class Server {
	private:
		//---------//
		i_tcp::endpoint endpoint_data;
		i_asio::io_context& lcontext;
		//acceptor accepts connections 
		i_tcp::acceptor lacceptor{ lcontext };
		//---------//
		std::set<std::shared_ptr<Session>> session;
		//---------//
		// An instant of the DB 
		//std::shared_ptr<MySQL_DB_Connect::mysql_connect> db;

		bool lisActive = false;
		uint32_t id_counter{ 0 }; 
	private:
		std::optional<bool> Server_isConnected(i_tcp::socket& lsocket) {
			auto status = lsocket.is_open();
			if ( !status ) {
				lisActive = true; 
				return true;
			}
			else if (status) {
				return false;
			}
			else {
				return std::nullopt;
			}
		}
	public:
		//---------//
		Server() = default;
		Server(i_asio::io_context& ucontext, const i_tcp::endpoint& uendpoint)
			: lcontext(ucontext), lacceptor(lcontext ) , endpoint_data ( uendpoint )
		{
			uint32_t id{ 0 }; 
			auto current_session = std::make_shared<Session>(id , lcontext);
			id_counter++;
			session.insert(current_session);
			lacceptor.open(uendpoint.protocol());
			//this sets the option to reuse the address
			lacceptor.set_option(i_asio::socket_base::reuse_address(true));
			lacceptor.bind(uendpoint);
			lacceptor.listen();
		}
		//------------------//
		//Server(i_asio::io_context& ucontext, const i_tcp::endpoint& uendpoint, std::shared_ptr<MySQL_DB_Connect::mysql_connect> ndb)
		//	: lcontext(ucontext), lacceptor(lcontext), endpoint_data(uendpoint), db(std::move(ndb))
		//{
		//	uint32_t id{ 0 }; 
		//	auto current_session = std::make_shared<Session>(id , lcontext);
		//	id_counter++;
		//	session.insert(current_session);
		//	lacceptor.open(uendpoint.protocol());
		//	//this sets the option to reuse the address
		//	lacceptor.set_option(i_asio::socket_base::reuse_address(true));
		//	lacceptor.bind(uendpoint);
		//	lacceptor.listen();
		//}
		//---------//
		void Server_run() ;
		void Server_AsyncAcceptConnection();
		void Server_ReadClientData(std::shared_ptr<Session> session);
		bool Server_DataReadChuck(i_tcp::socket& socket, size_t chunk_size);
		void Server_get_the_config() const;
		int Server_Handle_HTTP_request_type(std::shared_ptr<Session> session__, const std::string& msg, const i_http::status& st = i_http::status::ok) const;
	};
	//---------//
	void Server::Server_AsyncAcceptConnection() {
		try {
			auto session_ = std::make_shared<Session>(this->id_counter, lcontext);

			lacceptor.async_accept(session_->socket,
				[this, session_](boost::system::error_code ec) {
					if (!ec) {
						auto remote_end = session_->socket.remote_endpoint();
						std::cout << "Accepted connection from "
							<< remote_end.address().to_string()
							<< ":" << remote_end.port() << std::endl;
						this->id_counter++;
						// Store the shared_ptr, not a copy/move of Session
						this->session.insert(session_);

						Server_ReadClientData(session_);
					} else {
						std::cerr << "Could not accept a connection: " << ec.message() << std::endl;
					}
					Server_AsyncAcceptConnection();
				});
		} catch (const i_system::system_error& errn) {
			std::cerr << "Could not accept a connection - " << errn.what() << " " << errn.code() << std::endl;
			throw;
		} catch (const std::exception& exc) {
			std::cerr << "The exception has been caught" << exc.what() << std::endl;
			throw;
		}
	}
	//--------//
	//---------/////----------////----------////----------////----------////----------////----------//
	inline void Server::Server_get_the_config() const {
		boost::asio::ip::tcp::acceptor::reuse_address option; 
		this->lacceptor.get_option(option);

		std::cout << "Servers ip : " << this->endpoint_data.address()
			<< "Servers port : " << this->endpoint_data.port()
			<< "ServerSocketisValid : " << option.value() << std::endl;
	}
	inline int Server::Server_Handle_HTTP_request_type(std::shared_ptr<Session> session__,
		const std::string& msg, 
		const i_http::status& st) const {
		using HRV = i_http::verb ;
		try {
			const auto http_method = session__->request.method(); 
			switch (http_method) {
			case HRV::get:
				http_request_handle::http_GET_request_response(session__, msg, st);
			case HRV::post:
				http_request_handle::http_POST_request_response(session__, msg, st);
			case HRV::delete_:
				http_request_handle::http_DELETE_request_response(session__, msg, st);
			}
			return 1;
		}
		catch (const std::exception& exc) {
			std::cerr << "The exception has been caught - " << exc.what() << std::endl; 
			throw;
		}
		catch (const boost::system::system_error& err) {
			std::cerr << "The Boost_LIB :" << err.what() << " The code : " << err.code() << std::endl; 
			throw;
		}	
	}
	//----------////----------////----------////----------////----------////----------////----------//
	
	//----------////----------////----------////----------////----------////----------//
	void Server::Server_ReadClientData(std::shared_ptr<Session> session_) {
		boost::beast::http::async_read(
			session_->socket, session_->lmembuff, session_->request,
			[this , session_](boost::system::error_code ec, std::size_t bytes_transferred) {

				if (!ec) {
					//Server_handlehttpRequest(session_);
					Server_Handle_HTTP_request_type(session_, "Hello there!");

					if (session_->request.keep_alive()) {
						Server_ReadClientData(session_);
						auto remote_end = session_->socket.remote_endpoint();
						std::cout << "Accepted connection from "
							<< remote_end.address().to_string()
							<< ":" << remote_end.port() << std::endl;
					}
					else {
						i_system::error_code iec;
						session_->socket.shutdown(i_tcp::socket::shutdown_both, iec);
					}
					// optionally continue reading for persistent connections
					//Server_ReadClientData(session_);
				}
				else {
					std::cerr << "Read error: " << ec.message() << "Transfered :" << bytes_transferred << "\n";
				}
			});
	}
	//----------////----------////----------////----------////----------////----------////----------//
    inline void Server::Server_run() {

		std::cout << "Starting server : Listening on : " << endpoint_data.address() << " :: " << endpoint_data.port() << std::endl;

		Server_AsyncAcceptConnection(); 
		lcontext.run();

		std::cout << "Shutting down the server !" << std::endl;
	}
} 

#endif // CXX_CHAT_SERVER_DEF_HPP 