#include <iostream>
#include <optional>
#include <string>
#include "boost/beast/core.hpp"
#include "boost/beast.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio.hpp" 
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"
#include "Utils.cpp"
#include <memory>


namespace LoginRouteHadler {

	void GET_LoginRoutehandle(std::shared_ptr<Session> session_ , 
		const std::string_view& msg,
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

	void POST_LoginRouteHandle(std::shared_ptr<Session> session_,
		const std::string_view& msg,
		const i_http::status& stat = i_http::status::ok
	) {
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
	void DELETE_LoginRouetHandle(std::shared_ptr<Session> session_,
		const std::string_view& msg,
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
	void PATCH_LoginRouteHandle(std::shared_ptr<Session> session_,
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

