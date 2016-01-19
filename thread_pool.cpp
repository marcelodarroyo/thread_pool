#include "thread_pool.h"

// Constructor: launch workers
thread_pool::thread_pool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0; i<threads; ++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    while(!this->stop && this->tasks.empty())
                        this->condition.wait(lock);
                    if(this->stop && this->tasks.empty())
                        return;
                    std::function<void()> task(this->tasks.front());
                    this->tasks.pop();
                    lock.unlock();
                    task();
                }
            }
        );
}

// Destructor: join threads
thread_pool::~thread_pool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(size_t i = 0; i<workers.size(); ++i)
        workers[i].join();
}
