#include "boost/mysql.hpp"
#include "boost/asio.hpp"
#include <memory>
#include <iostream>
#include "../Header/DataStructures.hpp"
#include "../Header/MYSQL_DB_connect.hpp"

//---------//
namespace mysql = boost::mysql;
namespace asio = boost::asio;
//---------//

namespace MySQL_DB_Connect {

	mysql::results SQL_query_exec_res(const std::string& query , mysql_connect& db) {
		try {
			mysql::results res;  
			db.connect.execute(query, res);
			return res;
		}
		catch (const std::exception& exc) {
			std::cerr << "The exception has been caught during query exec : " << exc.what() << std::endl;
			throw;
		}
	}
}
 
namespace DB_worker {

	void dbworker::db_worker_process_loop() {
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
					auto res = MySQL_DB_Connect::SQL_query_exec_res(job.dbquery, *connection);
					if (!res.empty()) {
						std::cout << "Query succesfully executed : " << job.dbquery << " :" << std::endl;
					}
					else {
						std::cout << "Query succesfully executed, result is empty!" << std::endl;
					}
					job.dbtaskresult->set_value(res);
				}
			}
			catch (...) {
				if (job.dbtaskresult) {
					job.dbtaskresult->set_exception(std::current_exception());
				}
				try {
					std::rethrow_exception(std::current_exception());
				}
				catch (const std::exception& exc) {
					std::cerr << "Exception has been caught during DB workers runtime :" << exc.what() << std::endl;
					std::cerr << "The job hasnt been processed properly" << std::endl;
				}
				catch (const boost::system::error_code& errc) {
					std::cerr << "Exception of the Boost Library has been caught during DB workers runtime :"
						<< errc.message() << std::endl;
					std::cerr << "The job hasnt been processed properly" << std::endl;
				}
			}
		}
	}
	// add new jobs into the job pool 
	std::future<mysql::results> dbworker::dbworker_addjobs(std::string query) {
		try {
			std::shared_ptr<std::promise<mysql::results>> prom; 
			auto future = prom->get_future();
			{
				std::scoped_lock lock(mtx);
				// cerating dbtask object and pushing it into the jobs 
				// using emplace to build the dbtask object directly in the container 
				//dbtask task{ std::move(query), std::move(prom) };
				jobs.emplace( std::move(query), std::move(prom) );
			}
			cvar.notify_one();
			return future;
		}
		catch (const boost::system::system_error& errc ) {
			std::cerr << "The esception has been thrown in dbworker_addjobs function with"
				<< errc.code() << "code and " << errc.what() << " message." << std::endl;
			throw;
		}
	}
}