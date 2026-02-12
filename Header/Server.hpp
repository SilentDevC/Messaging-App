// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
// //Server definition header
//---------//
#ifndef CXX_CHAT_SERVER_DEF_HPP
#define CXX_CHAT_SERVER_DEF_HPP

#include <optional>
#include <string>
#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>
#include <set>
#include <initializer_list>
#include "boost/asio/ip/tcp.hpp"
#include "boost/json.hpp"
#include "nlohmann/json.hpp"
#include "Types.hpp"
#include "types.h"
#include "Logger.hpp"
#include "Utils.hpp"
#include "MYSQL_DB_connect.hpp"
//-----------//
namespace server_routing {
    using HRoute = std::function<void(std::shared_ptr<Session>)>;
    namespace http = boost::beast::http;
    
    //------------//
    const inline auto hRoutes =
        std::make_shared<std::unordered_map<std::string, HRoute>>(
            std::initializer_list<std::pair<const std::string, HRoute>>{
                { "login", HRoute() },
                { "users",  HRoute() },
                { "passwords", HRoute() }
            });
    //------------//
    enum class server_op_codes : int {
        create = 0,
        destroy = 1,
        update = 2
    };
    const inline auto routes = std::array<std::string, 3U> {
        "login" ,
        "users" ,
        "passwords"
    };
    using opcodes = server_routing::server_op_codes;
    //------------//
    const inline auto user_opcode_handler = std::unordered_map<opcodes, HRoute>{
            { opcodes::create,  HRoute() } , 
            { opcodes::destroy,  HRoute() } , 
            { opcodes::update,  HRoute() }
    };
    const inline auto login_opcode_handler = std::unordered_map<opcodes, HRoute>{
            { opcodes::create,  HRoute() } , 
            { opcodes::destroy,  HRoute() } , 
            { opcodes::update,  HRoute() }
    };
    const inline auto passwords_opcode_handler = std::unordered_map<opcodes, HRoute>{
            { opcodes::create,  HRoute() } , 
            { opcodes::destroy,  HRoute() } , 
            { opcodes::update,  HRoute() }
    };
    //------------//
    void server_request_handler(std::shared_ptr<Session>);
    HRoute handleLogin(); 
    HRoute handleUsers();
    HRoute handlePasswords();

}
//------------//
//internal Server namespace to isolate from potential errors 
namespace i_Server {
    //---------//
    std::vector<std::pair<std::future<mysql::results> , std::string>> db_exec_results; 
    //---------//
    class Server {
    private:
        //---------//
        i_tcp::endpoint endpoint_data;
        i_asio::io_context& lcontext;
        //acceptor accepts connections
        i_tcp::acceptor lacceptor{ lcontext };
        //---------//
        std::set<std::shared_ptr<Session>> session;
        //---------//
        bool lisActive = false;
        uint32_t id_counter{ 0 }; 
    private:
        std::optional<bool> Server_isConnected(i_tcp::socket& lsocket) { // socket type fixed // NOLINT 
            auto status = lsocket.is_open();
            if ( !status ) {
                lisActive = true; 
                return true;
            }
            else if (status) {
                return false;
            }
            else {
                return std::nullopt;
            }
        }
    public:
        //---------//
        Server() = default;
        Server(i_asio::io_context& ucontext, const i_tcp::endpoint& uendpoint)
            : endpoint_data(uendpoint), lcontext(ucontext), lacceptor(lcontext)
        {
            uint32_t id{ 0 }; 
            auto current_session = std::make_shared<Session>(id , lcontext);
            id_counter++;
            session.insert(current_session);
            lacceptor.open(uendpoint.protocol());
            //this sets the option to reuse the address
            lacceptor.set_option(i_asio::socket_base::reuse_address(true));
            lacceptor.bind(uendpoint);
            lacceptor.listen();
        }
        //------------------//
        // 
        //---------//
        void Server_run() ;
        void Server_AsyncAcceptConnection();
        void Server_ReadClientData(std::shared_ptr<Session> session);
        bool Server_DataReadChuck(i_tcp::socket& socket, size_t chunk_size); // fixed signature
        void Server_get_the_config() const;
        int Server_Handle_HTTP_request_type(std::shared_ptr<Session> session__, const std::string& msg, const i_http::status& st = i_http::status::ok) const;
    };
    //---------//
} 
#endif // CXX_CHAT_SERVER_DEF_HPP 