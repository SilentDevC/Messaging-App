//---------//
#ifndef CXX_CHAT_CLIENT_DEF_HPP
#define CXX_CHAT_CLIENT_DEF_HPP
#include <iostream>
#include <optional>
#include "boost/beast/core.hpp"
#include "boost/beast.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio.hpp" 
#include "boost/asio/ip/tcp.hpp"
#include "nlohmann/json.hpp"
#include "Header/Logger.hpp"
#include "Header/DataStructures.hpp"
#include "Source/Utils.cpp"
//-----------//


//-----------//
namespace i_Client {

	class Client {
	public:
		i_asio::io_context& lcontext;
		//acceptor accepts connections 
		i_tcp::acceptor lacceptor{ lcontext };

		std::vector<Session> sessions;

	private:

		Client() = default; 
		Client(i_asio::io_context& context)
			: lcontext(context) , lacceptor(context) {
			for (int i = 0; i < sessions.size(); i++ ) {
				sessions.at(i).lmembuff = boost::beast::flat_buffer();
				sessions.at(i).socket = boost::asio::ip::tcp::socket(context);
			}
		};

	void Client_AcceptConnection(boost::asio::ip::tcp::socket& sct, boost::asio::ip::tcp::endpoint& port) const;

	};

	
}



#endif //!CXX_CHAT_CLIENT_DEF_HPP
