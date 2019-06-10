#include <EventLoopThread.h>
#include <EventLoop.h>

using namespace sney;

EventLoopThread::EventLoopThread():
    _cond(_mutex)
{
}

EventLoopThread::~EventLoopThread()
{
}

bool EventLoopThread::start()
{   
    _thread.reset(new std::thread(std::bind(&EventLoopThread::run, this)));

    LockGuard lock(_mutex);
    if (_loop.get() == nullptr)
    {
        _cond.wait();
    }

    return true;
}

void EventLoopThread::stop()
{

}

void EventLoopThread::join()
{

}

void EventLoopThread::run()
{
    _loop = std::shared_ptr<EventLoop>(new EventLoop);

    {
        LockGuard lock(_mutex);
        _cond.notify();
    }

    _loop->loop();
}
