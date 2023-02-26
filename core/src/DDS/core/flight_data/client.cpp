#include <DDS/core/flight_data/client.hpp>
#include <DDS/core/flight_data/server.hpp>

FlightDataClient::FlightDataClient(FlightDataServer* s)
: server(s), run(true)
{
    handler = new std::thread([this]()
    {
        while (run)
        {
            std::unique_lock<std::mutex> lock(msg_qm);
            if (!msg_q.empty())
            {
                Message msg = msg_q.front();
                msg_q.pop();

                handle(msg);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
}

FlightDataClient::~FlightDataClient()
{
    run = false;
    if (handler->joinable())
        handler->join();
    delete handler;
}

void FlightDataClient::recv(std::string data)
{
    Message msg;
    msg.type = Message::INVALID;
    msg.data = data;

    recv(msg);
}

void FlightDataClient::recv(Message msg)
{
    std::unique_lock<std::mutex> lock(msg_qm);
    msg_q.push(msg);
}

void FlightDataClient::handle(Message msg)
{
    //server->broadcast(this, msg);
    //std::cout << msg << std::endl;
    if(msg.type == Message::HELLO)
    {
    	type = static_cast<Client::Type>(msg.data[0] - '0');
        id = 0;//todo
        msg.type = Message::HELLO_OK;
        msg.data = std::to_string(id);
    	
    }
    else if(msg.type == Message::DATA_BROADCAST)
    {
    	
    }

    response(msg);
}

void FlightDataClient::response(Message msg)
{
    if(msg.type == Message::HELLO_OK)
    {
        server->send(this, msg.data);
    }
    else if(msg.type == Message::DATA_BROADCAST)
    {
        server->broadcast(this, msg.data);
    }
}
