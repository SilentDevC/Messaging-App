#include <iostream>
#include <optional>
#include "boost/beast/core.hpp"
#include "boost/beast.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio.hpp" 
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"

namespace i_asio = boost::asio;
namespace i_beast = boost::beast;
namespace i_http = i_beast::http;
namespace i_system = boost::system;
using i_tcp = i_asio::ip::tcp;

struct Session {
	uint32_t id{ };
	i_beast::flat_buffer lmembuff{};
	i_tcp::socket socket;
	i_http::request<i_http::string_body> request;
	i_http::response<i_http::string_body> response;

	Session() = default;

	explicit Session(i_asio::io_context& ctx)
		: socket(ctx) {
	};

	explicit Session(uint32_t uid, i_asio::io_context& ctx)
		: id(uid), socket(ctx) {
	};

	void change_id(const uint32_t& uid) { id = uid; } 

	bool operator <(const Session& other) const {
		return this->id < other.id; 
	}
};

inline void read_result(boost::beast::flat_buffer& buf) {
	if (buf.size() == 0) {
		std::cerr << "There is no data read !" << std::endl;
	}
	if (buf.size() != 0) {
		std::cerr << "The data has been read : " << boost::beast::make_printable(buf.data()) << std::endl;
	}
}