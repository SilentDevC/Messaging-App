#include <optional>
#include <string>
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/json.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "../Header/Logger.hpp"
#include "../Header/Utils.hpp"
#include "../Header/Concurency.hpp"
#include "../Header/Server.hpp"
#include "../Header/MYSQL_DB_connect.hpp"
//---------//

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
		DB_worker::dbworker& worker,
		const std::string& msg,
		const i_http::status& st) const {
		using HRV = i_http::verb;
		try {
	//		server_routing::server_request_handler(session__, worker);
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
				//	Server_Handle_HTTP_request_type(session_, "Hello there!");

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
namespace sql_gen {
	//proffesional constexpr definition instead of doing #define s(T) std::string(T)
	template <typename T>
	[[nodiscard]] constexpr std::string s(T&& t) {
		return std::string(std::forward<T>(t));
	}

	std::string sql_user_create_query_gen(const user_data::u_full_data& data, boost::mysql::format_options opts) {
		// No manual quotes (') or string conversions (s()) needed!
		// The library handles types (bool, string, int) automatically.
		return boost::mysql::format_sql(opts,
			"INSERT INTO users (username, email, password_hash, created_at, is_active) "
			"VALUES ({}, {}, {}, {}, {})",
			data.username, data.email, data.hash, data.created_at, data.is_active
		);
	}

	std::string sql_user_update_query_gen(const user_data::u_full_data& data, boost::mysql::format_options opts) {
		// format_sql acts like std::format but specifically for SQL
		// It automatically adds single quotes and escapes dangerous characters
		return boost::mysql::format_sql(opts,
			"UPDATE users SET email={}, password_hash={}, created_at={}, is_active={} WHERE username={}",
			data.email,
			data.hash,
			data.created_at,
			data.is_active,
			data.username
		);
	}

	std::string sql_user_destroy_query_gen(const user_data::u_full_data& data, boost::mysql::format_options opts) {
		// format_sql automatically handles the single quotes around the username
		return boost::mysql::format_sql(opts, "DELETE FROM users WHERE username = {}", data.username);
	}
}
namespace sql_exec {
	bool server_create_user(DB_worker::dbworker& worker, const user_data::u_full_data& data) {
		try {
			auto query = sql_gen::sql_user_create_query_gen(data, worker.getoptions());
			auto query_res = worker.dbworker_addjobs(query);

			if (!query_res.valid()) {
				return false;
			}
			i_Server::db_exec_results.push_back({ std::move(query_res), query });
			return true;
		}
		catch (const boost::system::system_error& e) {
			if (e.code() == boost::mysql::common_server_errc::er_dup_entry) {
				std::cerr << "User already exists: " << data.username << std::endl;
			}
			else {
				std::cerr << "Database System Error: " << e.what() << std::endl;
			}
			return false;
		}
		catch (const std::exception& e) {
			std::cerr << "Standard Exception: " << e.what() << std::endl;
			return false;
		}
	}

	bool server_update_user(DB_worker::dbworker& worker, const user_data::u_full_data& data) {
		try {
			auto query = sql_gen::sql_user_update_query_gen(data, worker.getoptions());
			auto query_res = worker.dbworker_addjobs(query);

			if (!query_res.valid()) {
				return false;
			}
			i_Server::db_exec_results.push_back({ std::move(query_res), query });
			return true;
		}
		catch (const boost::system::system_error& e) {
			std::cerr << "Database Error during Update: " << e.what() << std::endl;
			return false;
		}
		catch (const std::exception& e) {
			std::cerr << "Standard Exception: " << e.what() << std::endl;
			return false;
		}
	}

	bool server_destroy_user(DB_worker::dbworker& worker, const user_data::u_full_data& data) {
		try {
			auto query = sql_gen::sql_user_destroy_query_gen(data, worker.getoptions());
			auto query_res = worker.dbworker_addjobs(query);

			if (!query_res.valid()) {
				return false;
			}
			i_Server::db_exec_results.push_back({ std::move(query_res), query });
			return true;
		}
		catch (const boost::system::system_error& e) {
			std::cerr << "Database Error during Destroy: " << e.what() << std::endl;
			return false;
		}
		catch (const std::exception& e) {
			std::cerr << "Standard Exception: " << e.what() << std::endl;
			return false;
		}
	}
}

namespace server_routing {
	
	namespace json = boost::json;

	user_data::u_basic_data tag_invoke(json::value_to_tag<user_data::u_basic_data>, json::value const& jval) {
		auto const& cref_obj = jval.as_object();
		user_data::u_basic_data bdata; 

		bdata.id = static_cast<int>(cref_obj.at("id").as_int64());
		bdata.username = cref_obj.at("username").as_string();
		bdata.hash = cref_obj.at("password_hash").as_string();

		return bdata;
	}

	user_data::u_full_data tag_invoke(json::value_to_tag<user_data::u_full_data> , json::value const& jval) {
		user_data::u_full_data full_data;

		static_cast<user_data::u_basic_data&>(full_data) = json::value_to<user_data::u_basic_data>(jval);

		auto const& obj = jval.as_object();
		full_data.email = obj.at("email").as_string();
		full_data.created_at = utils::string_to_date(obj.at("created_at").as_string());
		full_data.is_active = obj.at("is_active").as_bool();
		return full_data;
	}
	

	void parse_request_data(user_data::u_full_data& data, i_http::request<http::string_body>& req) {
		try {
			if (req[http::field::content_type].find("application/json") == std::string::npos) {
				std::cerr << "The response is not a JSON" << std::endl;
				return;
			}
			boost::system::error_code error;
			json::value jsonvalue = json::parse(req.body(), error);

			if (error) {
				std::cerr << "The parsing could not be done." << std::endl;
			}
			
			if (jsonvalue.is_object()) {
				auto const& obj = jsonvalue.get_object();

			}

		}
		catch (boost::mysql::error_with_diagnostics& err) {
			std::cerr << "Parse error , exception caught in data parser " << err.what() << " ,"
				<< err.get_diagnostics().server_message() << std::endl;
			return;
		}
	}

	template <typename _Ty>
	inline int server_comp_opcodes(server_routing::server_op_codes& opcode, auto code = std::string{ _Ty }) {
		if (!std::is_same_v<_Ty, std::string>) {
			std::cerr << "Error during opcode translation" << std::endl;
			return -1;
		}

	}

	void server_request_handler(std::shared_ptr<Session> ptr , DB_worker::dbworker& worker) {
		auto req_opcode = std::stoi(ptr->request["x_opcode"]);
		auto target = ptr->request["target"];
		user_data::u_full_data data;
		if ( target == "/users" ) {
			switch (req_opcode) {
				case static_cast<int>(opcodes::create):
					sql_exec::server_create_user(worker, data);
					break;
				case static_cast<int>(opcodes::update):
					sql_exec::server_update_user(worker, data);
					break;
				case static_cast<int>(opcodes::destroy):
					sql_exec::server_destroy_user(worker, data);
					break;
				default: 
					std::cout << "This action is not supported!" << std::endl;
					break;
			}
		}
	}
	
}
