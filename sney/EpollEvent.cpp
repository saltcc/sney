#include <EpollEvent.h>

using namespace sney;

EpollEvent::EpollEvent()
{
}


EpollEvent::~EpollEvent()
{

}

int EpollEvent::epollEventInit(int size, int ms)
{
    _size = size;
    _ms = ms;
    _quit = true;

    _events = std::unique_ptr<epoll_event[]>(new epoll_event[size]);
    _fileEvent = std::unique_ptr<FileEvent[]>(new FileEvent[size]);
    _readyEvent = std::unique_ptr<ReadyEvent[]>(new ReadyEvent[size]);

    _efd = epoll_create(1024);

    if (_efd == -1)
    {
        return -1;
    }

    return 0;
}

int EpollEvent::getSize()
{
    return _size;
}

int EpollEvent::addEvent(int fd, int mask)
{
    struct epoll_event ee = {0};
    
    int op = _fileEvent[fd].mask == E_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= _fileEvent[fd].mask;

    if (mask & E_READABLE)
        ee.events |= EPOLLIN;
    if (mask & E_WRITABLE)
        ee.events |= EPOLLOUT;

    ee.data.fd = fd;

    if (epoll_ctl(_efd, op, fd, &ee) == -1)
        return -1;

    return 0;
}

void EpollEvent::delEvent(int fd, int delmask)
{
    struct epoll_event ee = {0};

    int mask = _fileEvent[fd].mask & (~delmask);

    ee.events = 0;
    if (mask & E_READABLE)
        ee.events |= EPOLLIN;
    if (mask & E_WRITABLE)
        ee.events |= EPOLLOUT;

    ee.data.fd = fd;

    if (mask != E_NONE)
        epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &ee);
    else
        epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &ee);

    return;
}

int EpollEvent::wait()
{
    int retVal = 0;

    retVal = epoll_wait(_efd, _events.get(), _size, _ms);

    if (retVal <= 0)
        return 0;

    for (int i = 0; i < retVal; ++i)
    {
        int mask; 

        struct epoll_event *e = &_events[i];
        if (e->events & EPOLLIN)
            mask |= E_READABLE;
        if (e->events & EPOLLOUT)
            mask |= E_WRITABLE;
        if (e->events & EPOLLERR)
            mask |= E_WRITABLE;
        if (e->events & EPOLLHUP)
            mask |= E_WRITABLE;

        _readyEvent[i].fd = e->data.fd;
        _readyEvent[i].mask = mask;
    }

    return retVal;
}

void EpollEvent::eventDispatch()
{
    _quit = false;

    while (!_quit)
    {
        int numEvents = wait();

        for (int i = 0; i < numEvents; ++i)
        {
            int fd = _readyEvent[i].fd;

            FileEvent *fe = &_fileEvent[fd];         
            int mask = _readyEvent[i].mask;

            if (fe->mask & mask & E_READABLE)
                fe->readFunc(fd, mask);

            if (fe->mask & mask & E_WRITABLE)
                fe->writeFunc(fd, mask);
        }
    }

    return;
}

void EpollEvent::stop()
{
    _quit = true;
}
