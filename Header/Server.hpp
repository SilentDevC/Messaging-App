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
#include "nlohmann/json.hpp"
#include "Logger.hpp"
#include "Utils.hpp"                // provides Session and i_* aliases
#include "MYSQL_DB_Connect.hpp"

//-----------//
namespace server_routing {
    using HRoute = std::function<void(std::shared_ptr<Session>)>;
    //------------//
    const inline std::shared_ptr<std::unordered_map<std::string, HRoute>> hRoutes =
        std::make_shared<std::unordered_map<std::string, HRoute>>(
            std::initializer_list<std::pair<const std::string, HRoute>>{
                { "login", HRoute() },
                { "users",  HRoute() },
                { "passwords", HRoute() }
            }
        );
}
//------------//
//internal Server namespace to isolate from potential errors 
namespace i_Server {
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
        std::optional<bool> Server_isConnected(i_tcp::socket& lsocket) { // socket type fixed
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
            : lcontext(ucontext), lacceptor(lcontext) , endpoint_data(uendpoint)
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