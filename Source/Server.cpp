#include <optional>
#include <string>
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"
#include "boost/beast/websocket.hpp"
#include "../Header/Logger.hpp"
#include "../Header/Utils.hpp"
#include "../Header/Concurency.hpp"
#include "../Header/Server.hpp"

//---------//
namespace i_Server {

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
					}
					else {
						std::cerr << "Could not accept a connection: " << ec.message() << std::endl;
					}
					Server_AsyncAcceptConnection();
				});
		}
		catch (const i_system::system_error& errn) {
			std::cerr << "Could not accept a connection - " << errn.what() << " " << errn.code() << std::endl;
			throw;
		}
		catch (const std::exception& exc) {
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
		using HRV = i_http::verb;
		try {
			const auto http_method = session__->request.method();
			switch (http_method) {
			case HRV::get:
				//	http_request_handle::http_GET_request_response(session__, msg, st);
				break;
			case HRV::post:
				//	http_request_handle::http_POST_request_response(session__, msg, st);
				break;
			case HRV::delete_:
				//	http_request_handle::http_DELETE_request_response(session__, msg, st);
				break;
			default :
				break;
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
			[this, session_](boost::system::error_code ec, std::size_t bytes_transferred) {

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