#include <iostream>
#include "threadpool.h"
#include "tcphandler.h"

using namespace std;

class TCPEchoSession : public TCPSession{
public:
    TCPEchoSession(boost::asio::io_service &io_service) : TCPSession(io_service){}

private:
    virtual void _handler_tcp_read_some(const boost::system::error_code &err, std::size_t recv_size){
        if(!err){
            printf("TCP Echo : %.*s", (int)recv_size, get_tcp_data());
            TCPWrite(recv_size);
        }else
            TCPEndSession();
    }
    virtual void _handler_tcp_write(const boost::system::error_code &err){
        if(!err)
            TCPReadSome(TCPSession::kBufferSize);
        else
            TCPEndSession();
    }
};

void echo_tcp_accept(TCPHandler* ptr, const boost::system::error_code &err, TCPSession* tcp_session){
    if(!err){
        tcp_session->TCPReadSome(TCPSession::kBufferSize);
        ptr->TCPAccept(echo_tcp_accept, new TCPEchoSession(ptr->get_io_service()));
    }else
        tcp_session->TCPEndSession();
}

void nop(){
    int k = 0;
    k++;
}

int main(int argc, char *argv[])
{
    std::cout << "Thread Num : " << ThreadPool::get_instance().AddThread(4) << std::endl;
    TCPHandler tcp_handler(ThreadPool::get_instance().get_io_service(), (short)9998);
    TCPEchoSession *tcp_session = new TCPEchoSession(tcp_handler.get_io_service());
    tcp_handler.TCPAccept(echo_tcp_accept, tcp_session);

    for(int i = 0; i < 10000000; ++i)
        ThreadPool::get_instance().AddAsyncTask(nop);
    return 0;
}
