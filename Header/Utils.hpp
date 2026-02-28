#ifndef CXX_UTILS_HEADER
#define CXX_UTILS_HEADER

#include <string>
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/mysql.hpp"

namespace i_asio = boost::asio;
namespace i_beast = boost::beast;
namespace i_http = i_beast::http;
namespace i_system = boost::system;
using i_tcp = i_asio::ip::tcp;

struct Session {
    uint32_t id{ 0 };
    i_beast::flat_buffer lmembuff{};
    i_tcp::tcp::socket socket;
    i_http::request<i_http::string_body> request;
    i_http::response<i_http::string_body> response;

    Session(i_asio::io_context& ctx) : socket(ctx) {}
    Session(uint32_t uid, i_asio::io_context& ctx) : id(uid), socket(ctx) {}
    inline bool operator<(const Session& other) const { return id < other.id; }
    inline void change_id(const uint32_t& uid) { id = uid; }
};

namespace utils {
    inline void read_result(boost::beast::flat_buffer& buffer);
    std::string get_current_http_date();
    boost::mysql::date string_to_date(const std::string& str);
}

#endif 