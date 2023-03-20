#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>
#include <iostream>

#include <DDS/core/tcp_session.hpp>

template<class S>
class tcp_server : public std::enable_shared_from_this<tcp_server<S>>
{
    using tcp = boost::asio::ip::tcp;
public:
    tcp_server(boost::asio::io_context& io_context, uint16_t port = 1935)
    : ioc(io_context), acceptor(io_context)//, tcp::endpoint(tcp::v4(), port)
    {
        BOOST_STATIC_ASSERT(boost::is_base_of<tcp_session, S>::value);
        boost::system::error_code ec;
        tcp::endpoint endpoint(tcp::v4(), port);

        acceptor.open(endpoint.protocol(), ec);
        acceptor.set_option(tcp::acceptor::reuse_address(true));
        //acceptor.set_option(tcp::acceptor::keep_alive(true));
        acceptor.bind(endpoint, ec);
        acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
    }
    void run()
    {
        accept();
    }
private:
    boost::asio::io_context& ioc;
    tcp::acceptor acceptor;

    void accept()
    {
        auto self(this->shared_from_this());//
        acceptor.async_accept(boost::asio::make_strand(ioc), [self](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
                std::make_shared<S>(std::move(socket))->run();
            else
            {
                std::cerr << ec.message() << std::endl;
                return;
            }
            self->accept();
        });
    }
};

#endif
