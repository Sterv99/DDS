#include <boost/asio.hpp>
#include <DDS/config.hpp>
#include <DDS/core/client.hpp>

#ifdef COMPILE_WEBSOCKET
#include <DDS/websocket/server.hpp>
#endif

#ifdef COMPILE_RTMP
#include <DDS/rtmp/net/rtmp/rtmp_server.hpp>
#endif


int main()
{
    boost::asio::io_context io_context;

#ifdef COMPILE_WEBSOCKET
    WebsocketServer wss(&io_context); wss.run(5555);
#endif

#ifdef COMPILE_RTMP
    rtmp_server rtmp(io_context, 1935);
#endif

    io_context.run();

    return 0;
}
