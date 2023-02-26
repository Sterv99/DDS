#include <DDS/websocket/client.hpp>
//#include <DDS/core/fligt_data/server.hpp>
#include <iostream>


WebsocketClient::WebsocketClient(FlightDataServer* s)
    : FlightDataClient(s)
{

}

void WebsocketClient::recv(std::string data)
{
    Message msg;
    msg.type = Message::DATA_BROADCAST;
    msg.data = data;

    FlightDataClient::recv(msg);
}

void WebsocketClient::handle(Message msg)
{
    std::cout << msg.data << std::endl;

    FlightDataClient::handle(msg);
}

void WebsocketClient::response(Message msg)
{


    FlightDataClient::response(msg);
}