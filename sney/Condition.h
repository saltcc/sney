#pragma once

#include <Mutex.h>
#include <pthread.h>
#include <noncopyable.h>

namespace sney
{
    class Condition : private noncopyable
    {
        public:
            explicit Condition(Mutex &mutex) : _mutex(mutex)
            {
                pthread_cond_init(&_pcond, NULL);
            }

            ~Condition()
            {
                pthread_cond_destroy(&_pcond);
            }

            void wait()
            {
                pthread_cond_wait(&_pcond, &_mutex.get());
            }

            void notify()
            {
                pthread_cond_signal(&_pcond);
            }

            void notifyAll()
            {
                pthread_cond_broadcast(&_pcond);
            }
        private:
            Mutex &_mutex;
            pthread_cond_t _pcond;
    };
}
