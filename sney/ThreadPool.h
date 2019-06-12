#pragma once

#include <Thread.h>

#include <atomic>
#include <vector>

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
            inline EventLoop *getLoopByRoration();
            inline EventLoop *getLoopWithHash(uint64_t hash);
            inline uint32_t getThreadNum() const;

        private:
            EventLoop *_baseLoop;
            uint32_t _threadNum;
            std::atomic<uint64_t> _roration = {0};
            bool _start;

            typedef std::shared_ptr<Thread> ThreadPtr;
            std::vector<ThreadPtr> _threads;
    };
}
