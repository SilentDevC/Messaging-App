#ifndef CXX_MYSQL_CONN_HEADER
#define CXX_MYSQL_CONN_HEADER

#include "boost/mysql.hpp"
#include "boost/asio.hpp"
#include <memory>
#include <iostream>
#include "../Header/DataStructures.hpp"

namespace mysql = boost::mysql;
namespace asio = boost::asio;

namespace MySQL_DB_Connect {

	struct mysql_connect {
		//global io context for managing connections 
		asio::io_context& ctx;
		mysql::connect_params params;
		//represents the connection to the mysql sinstance 
		mysql::any_connection connect;

		mysql_connect() = default;

		mysql_connect(asio::io_context& m_ctx, const std::string& hostname, const short& port,
			const std::string& username, const std::string& password)
			: ctx(m_ctx), connect(ctx) {

			// conncetion parameters 
			params.server_address.emplace_host_and_port(hostname, port);
			params.username = username;
			params.password = password;
			//connect to the database
			connect.connect(params);
		}
		mysql_connect(const mysql_connect& x) = default;
		mysql_connect(mysql_connect&& x) = default;
		~mysql_connect() {
			connect.close();
		}
		mysql::results SQL_query_exec_res(const std::string& query, mysql_connect& db);
	};
}

namespace DB_worker {

	using std::function;

	struct dbtask {
		std::string dbquery{ };
		std::promise<mysql::results> dbtaskresult;
	};

	class dbworker {
	private:
		mutable std::mutex mtx;
		std::condition_variable cvar;
		std::thread th;
		std::queue<dbtask> jobs;
		MySQL_DB_Connect::mysql_connect connection;
		//shutdown variable 
		bool closed{ false };

		void db_worker_process_loop();

	public:
		dbworker(MySQL_DB_Connect::mysql_connect& n_connection)
			: connection(std::move(n_connection)) {
			// passing the reference to the current object which the thread is running 
			th = std::thread(&dbworker::db_worker_process_loop, this);
		};
		dbworker() = default;
		~dbworker() {
			{
				// using type deduction so we dont write <std::mutex>
				// for the scoped lock
				// scope lock can lock on multiple mutexes ( modern solution) 
				// ex. lk { mtx1 , mtx2 } 
				std::scoped_lock lk(mtx);
				closed = true;
			}
			cvar.notify_all();
			if (th.joinable()) {
				th.join();
			}
		}

		dbworker(const dbworker& x) noexcept = delete;
		dbworker(dbworker&& x) noexcept = delete;
		dbworker& operator=(const dbworker&) = delete;
		dbworker& operator=(dbworker&&) = delete;

		std::future<mysql::results> dbworker_addjobs(std::string& query);
		std::string_view db_get_last_error() const;
	};

}

#endif