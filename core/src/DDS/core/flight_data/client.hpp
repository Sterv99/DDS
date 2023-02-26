#ifndef FLIGHT_DATA_CLIENT_HPP
#define FLIGHT_DATA_CLIENT_HPP

#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>

#include <DDS/core/client.hpp>

struct Message
{
	enum Type
	{
		ERROR = -1,
		HELLO,
		HELLO_OK,
		DATA_BROADCAST,
		DRONE_LIST,
		DRONE_RTP,
		INVALID = 0xff
	} type;
	std::string data;
};

class FlightDataServer;
class FlightDataClient : public Client
{
public:
	FlightDataClient(FlightDataServer* s);
	virtual ~FlightDataClient();
	virtual void recv(std::string);
protected:
	void recv(Message);
	
	virtual void handle(Message);
	virtual void response(Message);
private:
	FlightDataServer* server;
	std::atomic<bool> run;
	std::thread* handler;

	std::queue<Message> msg_q;
	std::mutex msg_qm;
};

#endif // !FLIGHT_DATA_CLIENT_HPP
