#ifndef FLIGHT_DATA_SERVER_HPP
#define FLIGHT_DATA_SERVER_HPP

#include <DDS/core/client.hpp>
#include <DDS/core/flight_data/client.hpp>

class FlightDataServer
{
public:
	FlightDataServer() {}
	virtual ~FlightDataServer() {}

	virtual void run(uint16_t port) {}
	virtual void send(FlightDataClient* dst, const std::string& msg) {}
	virtual void broadcast(FlightDataClient* src, const std::string& msg) {}
};

#endif // !FLIGHT_DATA_SERVER_HPP
