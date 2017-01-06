#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <boost/serialization/singleton.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class ThreadPool : private boost::noncopyable
{
public:
    ThreadPool();
    ~ThreadPool();

    static ThreadPool& get_instance(){
        return boost::serialization::singleton<ThreadPool>::get_mutable_instance();
    }
    std::size_t get_thread_pool_size(){
        return thread_pool_size_;
    }
    boost::asio::io_service& get_io_service(){
        return io_service_;
    }

    std::size_t AddThread(std::size_t num_add_thread);

    template <typename TaskHandler>
    void AddAsyncTask(TaskHandler&& func_add_task){
        io_service_.post(func_add_task);
    }

private:
    friend class boost::serialization::singleton<ThreadPool>;

    std::size_t thread_pool_size_ = 0;
    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_;
    boost::asio::signal_set signal_set_;
    boost::thread_group thread_pool_;
};

#endif // THREADPOOL_H
