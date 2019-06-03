#pragma once

#include <noncopyable.h>
#include <vector>
#include <thread>
#include <memory>

namespace sney
{
    class EpollEvent;

    class EventLoop : private noncopyable
    {
        typedef std::function<void()> Functor;

        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void stop();

            void runInLoop();

            inline bool isInLoopThread() const;

            std::unique_ptr<EpollEvent> _epoll;
        private:

            std::vector<Functor> _pendingFunctor;
            std::thread::id _tid;
    };
}
