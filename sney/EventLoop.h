#pragma once

#include <noncopyable.h>
#include <Mutex.h>

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

            void runInLoop(const Functor &cb);
            void intoLoopQueue(const Functor &cb);

            void runInLoop(Functor &&cb);
            void intoLoopQueue(Functor &&cb);

            inline bool isInLoopThread() const;

            std::unique_ptr<EpollEvent> _epoll;

            void doPendingFunctors();
        private:

            std::vector<Functor> _pendingFunctor;
            std::thread::id _tid;
            mutable Mutex _mutex;
    };
}
