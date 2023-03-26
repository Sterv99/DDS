#include <iostream>
#include <exception>

#include <boost/asio.hpp>
#include <boost/exception/all.hpp>
#include <boost/program_options.hpp>

#include <DDS/config.hpp>
#include <DDS/core/logger.hpp>
#include <DDS/core/settings.hpp>

#ifdef COMPILE_WEBSOCKET
#include <DDS/websocket/server.hpp>
#endif

#ifdef COMPILE_RTMP
#include <DDS/core/tcp_server.hpp>
#include <DDS/rtmp/rtmp_session.hpp>
#endif

int run()
{
    auto& sett = settings::get();

    boost::asio::io_context io_context;
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    signals.async_wait([&io_context](const boost::system::error_code& error, int signum)
    {
        if(signum == SIGINT)
        {
            LOG(INFO) << "<DDS> " << "SIGINT received. Stopping server.";
        }
        else if(signum == SIGTERM)
        {
            LOG(INFO) << "<DDS> " << "SIGTERM received. Stopping server.";
        }
        io_context.stop();
    });


#ifdef COMPILE_WEBSOCKET
    try
    {
        std::make_shared<WebsocketServer>(&io_context)->run(sett.dint["websocket_port"]);
    }
    catch (websocketpp::exception const& e)
    {
        LOG(ERROR) << "<DDS> " << e.what();
        return -1;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "<DDS> " << e.what();
        return -1;
    }
#endif

#ifdef COMPILE_RTMP
    try
    {
        std::make_shared<tcp_server<rtmp_session>>(io_context)->run(sett.dint["rtmp_port"]);
    }
    catch (boost::exception &e)
    {
        LOG(ERROR) << "<DDS> " << boost::diagnostic_information(e);
        return -1;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "<DDS> " << e.what();
        return -1;
    }
#endif

    try
    {
        io_context.run();
    }
    catch (boost::exception &e)
    {
        LOG(ERROR) << "<DDS> " << boost::diagnostic_information(e);
        return -1;
    }
    catch (const std::exception& e)
    {
        LOG(ERROR) << "<DDS> " << e.what();
        return -1;
    }

    return 0;
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

#ifdef COMPILE_WEBSOCKET
        ("websocket_port,wp", po::value<int>(&sett.dint["websocket_port"])->default_value(5555), "Websocket Server port")
#endif

#ifdef COMPILE_RTMP
        ("rtmp_port,rp", po::value<int>(&sett.dint["rtmp_port"])->default_value(1935), "RTMP Server port")
#endif

        ("rec_width,rcw", po::value<unsigned>(&sett.record.width)->default_value(1920), "record width")
        ("rec_height,rch", po::value<unsigned>(&sett.record.height)->default_value(1080), "record height")
        ("rec_fps,rcf", po::value<int>(&sett.record.fps)->default_value(20), "record fps")
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

    return run();
}
