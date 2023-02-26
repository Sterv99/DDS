#ifndef WEBSOCKET_CLIENT_HPP
#define WEBSOCKET_CLIENT_HPP

#include <DDS/core/flight_data/client.hpp>

class FlightDataServer;
class WebsocketClient : public FlightDataClient
{
public:
    WebsocketClient(FlightDataServer*);
    ~WebsocketClient() {}
    void recv(std::string);
private:
    void handle(Message);
	void response(Message);
};

#endif