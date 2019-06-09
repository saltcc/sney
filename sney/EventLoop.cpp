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

void EventLoop::stop()
{
}

void EventLoop::runInLoop()
{

}

bool EventLoop::isInLoopThread()const
{
    return _tid == std::this_thread::get_id();
}
