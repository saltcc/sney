#pragma once

#include <noncopyable.h>
#include <Condition.h>
#include <Mutex.h>

#include <thread>

namespace sney
{
    class EventLoop;

    class Thread : private noncopyable
    {
        public:

            Thread();
            ~Thread();

            bool start();
            void stop();
            void join();
            void run();
            void setThreadName(const std::string &name);
            const std::string &getThreadName();
            std::thread::id getThreadId() const;
            EventLoop* getLoop() const;

        private:

            std::shared_ptr<EventLoop> _loop;
            std::shared_ptr<std::thread> _thread;
            std::string _name;

            Mutex _mutex;
            Condition _cond;
    };
}
