#include <EventLoop.h>
#include <EpollEvent.h>

using namespace sney;

EventLoop::EventLoop()
{
    _epoll = std::unique_ptr<EpollEvent>(new EpollEvent);
    _epoll->epollEventInit(1);
    _tid = std::this_thread::get_id();
}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
    _tid = std::this_thread::get_id();

    _epoll->eventDispatch();
}

void EventLoop::runInLoop(const Functor &cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        intoLoopQueue(cb);
    }
}

void EventLoop::intoLoopQueue(const Functor &cb)
{
    {
        LockGuard lock(_mutex);
        _pendingFunctor.push_back(cb);
    }
}

void EventLoop::runInLoop(Functor &&cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        intoLoopQueue(std::move(cb));
    }
}

void EventLoop::intoLoopQueue(Functor &&cb)
{
    {
        LockGuard lock(_mutex);
        _pendingFunctor.push_back(std::move(cb));
    }
}

void EventLoop::stop()
{
}

bool EventLoop::isInLoopThread()const
{
    return _tid == std::this_thread::get_id();
}

//by wakeup
void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    
    {
        LockGuard lock(_mutex);
        functors.swap(_pendingFunctor);
    }

    for (size_t i = 0; i < functors.size(); ++i)
    {
        functors[i]();
    }
}
