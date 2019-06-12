#include <ThreadPool.h>
#include <EventLoop.h>

#include <sstream>
#include <assert.h>

using namespace sney;

ThreadPool::ThreadPool(EventLoop *base, uint32_t threadNum):
    _baseLoop(base),
    _threadNum(threadNum),
    _start(false)
{

}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::start()
{
    for (uint32_t i = 0; i < _threadNum; ++i)
    {
        ThreadPtr t(new Thread);

        std::stringstream ss;
        ss<<"ThreadPool:thread["<<i<<"]";
        t->setThreadName(ss.str());

        _threads.push_back(t);
    }

    _start = true;

    return;
}

void ThreadPool::stop()
{
    for (auto &t : _threads)
    {
        t->stop();
    }   
    _start = false;
}

void ThreadPool::join()
{
    for (auto &t : _threads)
    {
        t->join();
    }
    _threads.clear();
}

EventLoop *ThreadPool::getLoopByRoration()
{
    assert(_start);

    EventLoop *loop = _baseLoop;
    
    if (!_threads.empty())
    {
        uint64_t v = _roration.fetch_add(1);

        v %= _threadNum;

        loop = (_threads[v])->getLoop();
    }

    return loop;
}

EventLoop *ThreadPool::getLoopWithHash(uint64_t hash)
{
    assert(_start);

    EventLoop *loop = _baseLoop;

    if (!_threads.empty())
    {
        uint64_t v = hash % _threadNum;

        loop = (_threads[v])->getLoop();
    }

    return loop;
}

uint32_t ThreadPool::getThreadNum() const
{
    return _threadNum;
}
