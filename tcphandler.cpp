#include "tcphandler.h"

void TCPSession::TCPReadSome(std::size_t read_size){
    tcp_sock_.async_read_some(boost::asio::buffer(tcp_data_, read_size),
                              boost::bind(&TCPSession::_handler_tcp_read_some, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}
void TCPSession::TCPRead(std::size_t read_size){
    boost::asio::async_read(tcp_sock_, boost::asio::buffer(tcp_data_, read_size),
                            boost::bind(&TCPSession::_handler_tcp_read, this,
                                        boost::asio::placeholders::error));
}
void TCPSession::TCPWriteSome(std::size_t write_size){
    tcp_sock_.async_write_some(boost::asio::buffer(tcp_data_, write_size),
                              boost::bind(&TCPSession::_handler_tcp_write_some, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}
void TCPSession::TCPWrite(std::size_t write_size){
    boost::asio::async_write(tcp_sock_, boost::asio::buffer(tcp_data_, write_size),
                            boost::bind(&TCPSession::_handler_tcp_write, this,
                                        boost::asio::placeholders::error));
}
void TCPSession::TCPReadSome(SomeFuncHandler handler, std::size_t read_size){
    tcp_sock_.async_read_some(boost::asio::buffer(tcp_data_, read_size),
                              boost::bind(handler, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}
void TCPSession::TCPRead(FuncHandler handler, std::size_t read_size){
    boost::asio::async_read(tcp_sock_, boost::asio::buffer(tcp_data_, read_size),
                            boost::bind(handler, this,
                                        boost::asio::placeholders::error));
}
void TCPSession::TCPWriteSome(SomeFuncHandler handler, std::size_t write_size){
    tcp_sock_.async_write_some(boost::asio::buffer(tcp_data_, write_size),
                              boost::bind(handler, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
}
void TCPSession::TCPWrite(FuncHandler handler, std::size_t write_size){
    boost::asio::async_write(tcp_sock_, boost::asio::buffer(tcp_data_, write_size),
                            boost::bind(handler, this,
                                        boost::asio::placeholders::error));
}

TCPSession* TCPHandler::TCPAccept(TCPSession* tcp_session){
    tcp_acceptor_.async_accept(tcp_session->get_tcp_sock(),
                               boost::bind(&TCPHandler::_handler_tcp_accept, this,
                                           boost::asio::placeholders::error,
                                           tcp_session));
    return tcp_session;
}

TCPSession* TCPHandler::TCPAccept(AcceptHandler handler, TCPSession* tcp_session){
    tcp_acceptor_.async_accept(tcp_session->get_tcp_sock(),
                               boost::bind(handler, this,
                                           boost::asio::placeholders::error,
                                           tcp_session));
    return tcp_session;
}
