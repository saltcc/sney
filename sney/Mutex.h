#pragma once

#include <pthread.h>
#include <noncopyable.h>

namespace sney
{
    class Mutex : private noncopyable
    {
        public:
            Mutex()
            {
                pthread_mutex_init(&_pmutex, NULL);
            }

            ~Mutex()
            {
                pthread_mutex_destroy(&_pmutex);
            }

            void lock()
            {
                pthread_mutex_lock(&_pmutex);
            }

            void unlock()
            {
                pthread_mutex_unlock(&_pmutex);
            }

            pthread_mutex_t &get()
            {
                return _pmutex;
            }

        private:
            pthread_mutex_t _pmutex;
    };

    class LockGuard : private noncopyable
    {
        public:
            explicit LockGuard(Mutex &mutex) : _mutex(mutex)
            {
                _mutex.lock();
            }

            ~LockGuard()
            {
                _mutex.unlock();
            }

        private:
            Mutex &_mutex;
    };
}
