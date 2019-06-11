#include <Thread.h>
#include <EventLoop.h>

#include <system_error>
#include <iostream>

using namespace sney;

Thread::Thread():
    _cond(_mutex)
{
}

Thread::~Thread()
{
    join();
}

bool Thread::start()
{   
    _thread.reset(new std::thread(std::bind(&Thread::run, this)));

    LockGuard lock(_mutex);
    while (_loop.get() == nullptr)
    {
        _cond.wait();
    }

    return true;
}

void Thread::stop()
{
    _loop->stop();

    join();
}

void Thread::join()
{
    LockGuard lock(_mutex);

    if (_thread && _thread->joinable())
    {
        try
        {
            _thread->join();
        }
        catch (const std::system_error& e)
        {
            std::cerr<<"catch system error: "<<e.what()<<" error code:"<<e.code()<<std::endl;
        }
        _thread.reset();
    }
}

void Thread::run()
{
    _loop = std::shared_ptr<EventLoop>(new EventLoop);

    {
        LockGuard lock(_mutex);
        _cond.notify();
    }

    _loop->loop();
}
