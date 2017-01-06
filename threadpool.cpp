#include "threadpool.h"

ThreadPool::ThreadPool() : work_(io_service_), signal_set_(io_service_){
    signal_set_.add(SIGINT);
    signal_set_.add(SIGTERM);

#if defined(SIGQUIT)
    signal_set_.add(SIGQUIT);
#endif
    signal_set_.async_wait(
                boost::bind(&boost::asio::io_service::stop,
                            &io_service_));
}

ThreadPool::~ThreadPool(){
    io_service_.stop();
    try{
        thread_pool_.join_all();
    }catch(const std::exception& e){
        std::runtime_error(e.what());
    }
}

std::size_t ThreadPool::AddThread(std::size_t num_add_thread){
    std::size_t inc_pool_size = 0;

    for(std::size_t i = 0; i < num_add_thread; ++i){
        if(thread_pool_.create_thread(
                    boost::bind(&boost::asio::io_service::run,
                                &io_service_)) == nullptr)
            break;
        inc_pool_size++;
    }
    thread_pool_size_ += inc_pool_size;
    return inc_pool_size;
}
