#pragma once

#include <Thread.h>

namespace sney
{
    class EventLoop;

    class ThreadPool
    {
        public:
            ThreadPool(EventLoop *base, uint32_t threadNum);
            ~ThreadPool();
            void start();
            void stop();
            void join();
            EventLoop *getLoopByRoration();
            EventLoop *getLoopWithHash(uint64_t hash);
            uint32_t getThreadNum() const;

        private:
            EventLoop *_baseLoop;
            uint32_t _threadNum;
            std::atomic<uint64_t> _roration;
            uint32_t _threadNum;

            typedef std::shared_ptr<Thread> ThreadPtr;
            std::vector<ThreadPtr> _threads;
    };
}
