#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class TCPSession{
public:
    typedef boost::function<void(TCPSession*, const boost::system::error_code&, std::size_t)> SomeFuncHandler;
    typedef boost::function<void(TCPSession*, const boost::system::error_code&)> FuncHandler;

    static constexpr int kBufferSize = 4096;

    TCPSession(boost::asio::io_service &io_service) : tcp_sock_(io_service){}

    virtual ~TCPSession(){}

    boost::asio::ip::tcp::socket& get_tcp_sock(){
        return tcp_sock_;
    }
    std::uint8_t* get_tcp_data(){
        return tcp_data_;
    }

    void TCPReadSome(std::size_t read_size);
    void TCPRead(std::size_t read_size);
    void TCPWriteSome(std::size_t write_size);
    void TCPWrite(std::size_t write_size);

    void TCPReadSome(SomeFuncHandler handler, std::size_t read_size);
    void TCPRead(FuncHandler handler, std::size_t read_size);
    void TCPWriteSome(SomeFuncHandler handler, std::size_t write_size);
    void TCPWrite(FuncHandler handler, std::size_t write_size);

    void TCPEndSession(){
        delete this;
    }

private:
    virtual void _handler_tcp_read(const boost::system::error_code &err){}
    virtual void _handler_tcp_write(const boost::system::error_code &err){}
    virtual void _handler_tcp_read_some(const boost::system::error_code &err, std::size_t recv_size){}
    virtual void _handler_tcp_write_some(const boost::system::error_code &err, std::size_t send_size){}

    boost::asio::ip::tcp::socket tcp_sock_;
    std::uint8_t tcp_data_[kBufferSize];
};

class TCPHandler
{
public:
    typedef boost::function<void(TCPHandler*, const boost::system::error_code&, TCPSession*)> AcceptHandler;

    TCPHandler(boost::asio::io_service &io_service, short port)
        : io_service_(io_service),
          tcp_acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(), port)){}

    virtual ~TCPHandler(){}

    boost::asio::io_service& get_io_service(){
        return io_service_;
    }

    TCPSession* TCPAccept(){
        return TCPAccept(new TCPSession(io_service_));
    }
    TCPSession* TCPAccept(AcceptHandler handler){
        return TCPAccept(handler, new TCPSession(io_service_));
    }

    TCPSession* TCPAccept(TCPSession *tcp_session);
    TCPSession* TCPAccept(AcceptHandler handler, TCPSession *tcp_session);

private:
    virtual void _handler_tcp_accept(const boost::system::error_code &err, TCPSession* tcp_session){}

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor tcp_acceptor_;
};

#endif //TCPHANDLER_H
