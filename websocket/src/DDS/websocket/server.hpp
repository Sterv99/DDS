#ifndef WEBSOCKET_SERVER_HPP
#define WEBSOCKET_SERVER_HPP

#include <map>
#include <boost/asio.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <DDS/core/flight_data/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server_t;

class Client;
class WebsocketServer : public FlightDataServer //: public std::enable_shared_from_this<WebsocketServer>
{
public:
	WebsocketServer(boost::asio::io_context*);
	void run(uint16_t);
	void send(FlightDataClient*, const std::string& msg);
	void broadcast(FlightDataClient*, const std::string& msg);
private:
	server_t server;

	std::map<websocketpp::connection_hdl, FlightDataClient*, std::owner_less<websocketpp::connection_hdl>> clients;

	void on_open(websocketpp::connection_hdl);
	void on_close(websocketpp::connection_hdl);
	void on_message(websocketpp::connection_hdl, server_t::message_ptr);
};

#endif // !SERVER_HPP
