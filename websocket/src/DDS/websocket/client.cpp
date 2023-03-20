#include <DDS/websocket/client.hpp>
#include <DDS/websocket/json.hpp>
#include <DDS/core/flight_data/server.hpp>

using json = nlohmann::json;

WebsocketClient::WebsocketClient(std::shared_ptr<FlightDataServer> s)
    : FlightDataClient(s)
{

}

void WebsocketClient::recv(std::string msg)
{
    add(msg);
}

void WebsocketClient::handle(std::string msg)
{
    //server->broadcast(this, msg);///
    //return;///



    json j = json::parse(msg);

    if(j["type"] == "hello")
    {
        hello(j["data"]["ctype"], j["data"]["drone_name"], j["data"]["serial"]);
    }
    else if(j["type"] == "data_broadcast")
    {
        data(j.dump());
    }
    else if(j["type"] == "drone_list")
    {
        drone_list();
    }
    else
    {

    }
}

void WebsocketClient::on_hello(ClientID_t cid)
{
    json j =
    {
        {"type", "hello_resp"},
        {"data",
            {
                {"cid", cid_to_hex(cid)}
            }
        }
    };

    if(this->type == Client::Type::DRONE)
    {
        j["data"]["rtmp_url"] = "";
    }

    server->send(this, j.dump());
}

void WebsocketClient::on_data()
{

}

void to_json(json& j, const Client* c)
{
    j =
    {
        {"cid", cid_to_hex(c->id)},
        {"drone_name", c->drone_name},
        {"serial", c->serial}
    };
}

void WebsocketClient::on_drone_list(std::vector<Client*>& drones)
{
    json j =
    {
        {"type", "drone_list_resp"}
    };
    j["data"] = json(drones);

    server->send(this, j.dump());
}