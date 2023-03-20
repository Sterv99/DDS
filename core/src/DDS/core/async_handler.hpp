#ifndef ASYNC_HANDLER_HPP
#define ASYNC_HANDLER_HPP

#include <atomic>
#include <thread>
#include <queue>
#include <mutex>

template<class T>
class async_handler
{
    std::atomic<bool> run;
	std::thread* handler;
	std::queue<T> msg_q;
	std::mutex msg_qm;
public:
    virtual ~async_handler()
    {
        run = false;
        if (handler->joinable())
            handler->join();
        delete handler;
    }
    void add(T j)
    {
        std::unique_lock<std::mutex> lock(msg_qm);
        msg_q.push(j);
    }
protected:
    virtual void handle(T) {}
    async_handler()
    : run(true)
    {
        handler = new std::thread([this]()
        {
            while (run)
            {
                std::unique_lock<std::mutex> lock(msg_qm);
                if (!msg_q.empty())
                {
                    T j = msg_q.front();
                    msg_q.pop();

                    handle(j);
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }
};

#endif