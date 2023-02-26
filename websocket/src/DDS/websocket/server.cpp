#include <DDS/websocket/server.hpp>
//#include "client.hpp"
#include <DDS/websocket/client.hpp>
#include <functional>

WebsocketServer::WebsocketServer(boost::asio::io_context* io)
{
    server.clear_access_channels(websocketpp::log::alevel::all);
    server.init_asio(io);

    server.set_open_handler(std::bind(&WebsocketServer::on_open, this, std::placeholders::_1));
    server.set_close_handler(std::bind(&WebsocketServer::on_close, this, std::placeholders::_1));
    server.set_message_handler(std::bind(&WebsocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void WebsocketServer::run(uint16_t port)
{
    server.listen(port);
    server.start_accept();
}

void WebsocketServer::send(FlightDataClient* dst, const std::string& msg)
{
    for (auto& c : clients)
        if (c.second == dst)
            server.send(c.first, msg, websocketpp::frame::opcode::TEXT);
}

void WebsocketServer::broadcast(FlightDataClient* src, const std::string& msg)
{
    for (auto& c : clients)
        if (c.second != src)
            server.send(c.first, msg, websocketpp::frame::opcode::TEXT);
}

void WebsocketServer::on_open(websocketpp::connection_hdl conn)
{
    if (clients[conn])
        delete clients[conn];
    auto xd = new WebsocketClient(this);
    clients[conn] = xd;
    
    //cpool->at(clients[conn]->id) = *clients[conn];
}

void WebsocketServer::on_close(websocketpp::connection_hdl conn)
{
    //cpool->erase(clients[conn]->id);

    delete clients[conn];
}

void WebsocketServer::on_message(websocketpp::connection_hdl conn, server_t::message_ptr msg)
{
    clients[conn]->recv(msg->get_payload());
    /*for (auto& c : clients)
    {
        if(c.first.owner_before(conn) || conn.owner_before(c.first))
            server.send(c.first, msg);
    }*/
}
