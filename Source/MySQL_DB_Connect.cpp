#include "boost/mysql.hpp"
#include "boost/asio.hpp"
#include <memory>
#include <iostream>
#include "../Header/DataStructures.hpp"

//namespace mysql = boost::mysql;
namespace asio = boost::asio;

namespace MySQL_DB_Connect {

	//struct mysql_connect {
	//	//global io context for managing connections 
	//	asio::io_context& ctx;
	//	mysql::connect_params params; 
	//	//represents the connection to the mysql sinstance 
	//	mysql::any_connection connect; 

	//	mysql_connect() = default; 

	//	mysql_connect(asio::io_context& m_ctx, const std::string& hostname, const short& port,
	//		const std::string& username, const std::string& password)
	//		: ctx(m_ctx), connect(ctx) {
	//
	//		// conncetion parameters 
	//		params.server_address.emplace_host_and_port(hostname, port);
	//		params.username = username; 
	//		params.password = password;
	//		//connect to the database
	//		connect.connect(params); 
	//	}
	//	mysql_connect(const mysql_connect& x) = default;
	//	mysql_connect(mysql_connect&& x) = default;
	//	~mysql_connect() {
	//		connect.close();
	//	}
	//};

	//mysql::results SQL_query_exec_res(const std::string& query , mysql_connect& db) {
	//	try {
	//		mysql::results res;  
	//		db.connect.execute(query, res);
	//		if (!res.empty()) return res;
	//		else return mysql::results();
	//	}
	//	catch (const std::exception& exc) {
	//		std::cerr << "The exception has been caught during query exec : " << exc.what() << std::endl;
	//		throw;
	//	}
	//}
}
 
namespace DB_worker {

	using std::function;

	struct dbtask {
		std::string dbquery{ }; 
		//std::promise<mysql::results> dbtaskresult; 
	};

	class dbworker {
	private:
		mutable std::mutex mtx;
		std::condition_variable cvar;
		std::thread th;
		std::queue<dbtask> jobs;
		//MySQL_DB_Connect::mysql_connect connection;
		//shutdown variable 
		bool closed{ false };

		void db_worker_process_loop() {
			while (true) {
				dbtask job; 
				{
					std::unique_lock ulk(mtx);
					cvar.wait(ulk, [this]() {return closed || !jobs.empty(); });
					if (closed && jobs.empty()) {
						return;
					}
					job = std::move(jobs.front());
					jobs.pop();
				}
				try {
					// if there is a job execute it 
					if (!job.dbquery.empty()) {
						//auto res = MySQL_DB_Connect::SQL_query_exec_res(job.dbquery, connection);
						std::cout << "Query succesfully executed : " << job.dbquery << " :" << std::endl; 
						//job.dbtaskresult.set_value(res);
					}
				}
				catch (const std::exception& exc) {
					std::cerr << "Exception has been caught during DB workers runtime :" << exc.what() << std::endl;
					std::cerr << "The job hasnt been processed properly" << std::endl;
				}
				catch (boost::system::error_code& errc) {
					std::cerr << "Exception of the Boost Library has been caught during DB workers runtime :"
						<< errc.message() << std::endl;	
					std::cerr << "The job hasnt been processed properly" << std::endl;
				}
			}
		}

	public:
		//dbworker(MySQL_DB_Connect::mysql_connect& n_connection)
		//	: connection(std::move(n_connection)) {
		//	// passing the reference to the current object which the thread is running 
		//	th = std::thread(&dbworker::db_worker_process_loop, this);
		//};
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

		//std::future<mysql::results> dbworker_addjobs(std::string& query);
		std::string_view db_get_last_error() const;
	};
	// add new jobs into the job pool 
	//std::future<mysql::results> dbworker::dbworker_addjobs(std::string& query) {
	//	try {
	//		std::promise<mysql::results> prom; 
	//		auto future = prom.get_future();
	//		{
	//			std::scoped_lock lock(mtx);
	//			// cerating dbtask object and pushing it into the jobs 
	//			jobs.push(dbtask{ std::move(query), std::move(prom) });
	//		}
	//		
	//		cvar.notify_one();
	//		return future;
	//	}
	//	catch (const boost::system::system_error& errc ) {
	//		std::cerr << "The esception has been thrown in dbworker_addjobs function with"
	//			<< errc.code() << "code and " << errc.what() << " message." << std::endl;
	//	}
	//}
}