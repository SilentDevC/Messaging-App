#include <iostream>
#include <optional>
#include "boost/beast/core.hpp"
#include "boost/beast.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio.hpp" 
#include "boost/asio/ip/tcp.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "../Header/Utils.hpp"

namespace utils {

	inline void read_result(boost::beast::flat_buffer& buf) {
		if (buf.size() == 0) {
			std::cerr << "There is no data read !" << std::endl;
		}
		if (buf.size() != 0) {
			std::cerr << "The data has been read : " << boost::beast::make_printable(buf.data()) << std::endl;
		}
	}

	std::string get_current_http_date() {
		using namespace std::chrono;
		auto now = system_clock::now();
		std::time_t now_time = system_clock::to_time_t(now);
		std::tm tm;
#if defined(_WIN32) || defined(_WIN64)
		gmtime_s(&tm, &now_time);
#else
		gmtime_r(&now_time, &tm);
#endif
		std::ostringstream oss;
		oss << std::put_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
		return oss.str();
	}

	boost::mysql::date string_to_date(const std::string& str) {
		return boost::mysql::date(
			static_cast<uint16_t>(boost::gregorian::from_string(str).year()),
			static_cast<uint8_t>(boost::gregorian::from_string(str).month()),
			static_cast<uint8_t>(boost::gregorian::from_string(str).day())
		);
	}
}