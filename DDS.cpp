#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include <DDS/config.hpp>
#include <DDS/core/settings.hpp>

#ifdef COMPILE_WEBSOCKET
#include <DDS/websocket/server.hpp>
#endif

#ifdef COMPILE_RTMP
#include <DDS/core/tcp_server.hpp>
#include <DDS/rtmp/rtmp_session.hpp>
#endif


void run()
{
    boost::asio::io_context io_context;

#ifdef COMPILE_WEBSOCKET
    std::make_shared<WebsocketServer>(&io_context)->run(5555);
#endif

#ifdef COMPILE_RTMP
    std::make_shared<tcp_server<rtmp_session>>(io_context)->run();
#endif

    io_context.run();
}

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    auto& sett = settings::get();

    int log_level;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("log,l", po::value<int>(&log_level)->default_value(2), "log level: 3-ERROR,2-WARNING,1-INFO,0-DEBUG")

        ("rec_height,rh", po::value<unsigned>(&sett.record.height)->default_value(1920), "record height")
        ("rec_width,rw", po::value<unsigned>(&sett.record.width)->default_value(1080), "record width")
        ("rec_fps,rf", po::value<int>(&sett.record.fps)->default_value(20), "record fps")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    sett.set_loglevel(log_level);

    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    run();

    return 0;
}
