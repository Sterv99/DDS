#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>
#include <memory>

typedef uint32_t ClientID_t;

class Client
{
public:
	virtual ~Client() {}

	enum Type
	{
		INVALID = -1,
		DRONE,
		OPERATOR
	} type = INVALID;
	ClientID_t id;
};

typedef std::shared_ptr<std::map<ClientID_t, Client>> pClientPool;


#endif // !CLIENT_HPP
