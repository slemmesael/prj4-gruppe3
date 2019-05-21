#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <pthread.h>

namespace osapi
{
    class Mutex
    {
    public:
        Mutex();
        void lock();
        void unlock();
        pthread_mutex_t mut_;
        ~Mutex();
    private:
    };
}

#endif
