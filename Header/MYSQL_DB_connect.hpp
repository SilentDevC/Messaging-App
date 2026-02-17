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

	struct db_params {
	public:
		short port{ 3306 };
		std::string hostname{ "localhost" };
		std::string username{ "root" };
		std::string password{ "admin" };
		std::string database{ "users" };

		void db_params_custom(
			short port,
			std::string hostname,
			std::string username,
			std::string password,
			std::string database
		);
	};

	void db_params_init(const db_params& params, mysql::connect_params& sql_params);

	struct mysql_connect {
		//global io context for managing connections 
		asio::io_context* ctx;
		mysql::connect_params params;
		//represents the connection to the mysql sinstance 
		mysql::any_connection connect;
		db_params u_params;
		boost::mysql::format_options opts;
		mysql_connect() = default;

		//cunstructor with custom params 
		mysql_connect(asio::io_context& m_ctx, const std::string& hostname, const short& port,
			const std::string& username, const std::string& password , const std::string& database = "users")
			: ctx(&m_ctx), connect(*ctx) {
			u_params.db_params_custom(port, hostname, username, password, database);
			db_params_init(u_params, params);
			//connect to the database
			try {
				connect.connect(params);
				std::cout << "The connection established!" << std::endl;
				//format options needed to execute queries safely
				opts = this->connect.format_opts().value();
			}
			catch (const boost::mysql::error_with_diagnostics& err) {
				std::cerr << "Connection failed: " << err.what() << std::endl;
				std::cerr << "Server diagnostics: " << err.get_diagnostics().server_message() << std::endl;
				throw;
			}
		}
		// constructor with default params 
		mysql_connect(asio::io_context& m_ctx)
			: ctx(&m_ctx), connect(*ctx) {
			// conncetion parameters 
			db_params_init(u_params, params);
			//connect to the database
			try {
				connect.connect(params);
				std::cout << "The connection established!" << std::endl;
			}
			catch (const boost::mysql::error_with_diagnostics& err) {
				std::cerr << "Connection failed: " << err.what() << std::endl;
				std::cerr << "Server diagnostics: " << err.get_diagnostics().server_message() << std::endl;
				throw;
			}
		}

		mysql_connect(const mysql_connect&) = delete;
		mysql_connect& operator=(const mysql_connect&) = delete;

		mysql_connect(mysql_connect&&) = delete;
		mysql_connect& operator=(mysql_connect&&) = delete;

		~mysql_connect() {
			try {
				connect.close();
			} 
			catch (boost::mysql::error_code& errc) {
				std::cerr << "Caught an exception when releasing the resources!" << std::endl;
				std::cerr << "With message : " << errc.message() << std::endl;
			}
		}
	};
	mysql::results SQL_query_exec_res(const std::string& query, mysql_connect& db);
} // neamespace end 

namespace DB_worker {

	struct dbtask {
		std::string dbquery{ };
		std::shared_ptr<std::promise<mysql::results>> dbtaskresult;
	};

	class [[nodiscard("Do not ignore the return values of member functions of the dbworker class")]] dbworker {
	private:
		mutable std::mutex mtx;
		std::condition_variable cvar;
		std::jthread th;
		std::queue<dbtask> jobs;
		std::unique_ptr<MySQL_DB_Connect::mysql_connect> connection;
		boost::mysql::format_options opts;
		//shutdown variable 
		bool closed{ false };

		void db_worker_process_loop();

	public:
		dbworker(asio::io_context& m_ctx,
			const std::string& hostname, 
			const short& port,
			const std::string& username, 
			const std::string& password,
			const std::string& database = "users")
			: connection(std::make_unique<MySQL_DB_Connect::mysql_connect>
				(m_ctx , hostname , port , username , password , database)) {
			// passing the reference to the current object which the thread is running 
			opts = connection.get()->opts;
			th = std::jthread(&dbworker::db_worker_process_loop, this);
		};

		dbworker(asio::io_context& m_ctx)
			: connection(std::make_unique<MySQL_DB_Connect::mysql_connect>(m_ctx)) {
			opts = connection.get()->opts;
			th = std::jthread(&dbworker::db_worker_process_loop, this);
		};
		
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

		auto getoptions() const { return opts; }

		dbworker(const dbworker& x) noexcept = delete;
		dbworker(dbworker&& x) noexcept = delete;
		dbworker& operator=(const dbworker&) = delete;
		dbworker& operator=(dbworker&&) = delete;

		// passing the query by value 
		std::future<mysql::results> dbworker_addjobs(std::string query);
		std::string_view db_get_last_error() const;
	};

}
#endif