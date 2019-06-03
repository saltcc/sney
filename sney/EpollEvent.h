#pragma once

#include <memory>
#include <sys/epoll.h>

namespace sney
{
    enum EventType
    {
        E_NONE = 0,
        E_READABLE,
        E_WRITABLE
    };

    typedef std::function<void(int fd, int mask)> ReadEventCallBack;
    typedef std::function<void(int fd, int mask)> WriteEventCallBack;

    struct ReadyEvent
    {
        int fd;
        int mask;
    };

    struct FileEvent
    {
        FileEvent():
            mask(E_NONE)
        {
        }
        int mask;
        ReadEventCallBack readFunc;
        WriteEventCallBack writeFunc;
    };

    class EpollEvent
    {
        public:
            EpollEvent();
            ~EpollEvent();
            int epollEventInit(int size, int ms = -1);
            int getSize();

            int addEvent(int fd, int mask);
            void delEvent(int fd, int delmask);
            void eventDispatch();
            void stop();
        private:
            int _size;
            int _ms;
            int _efd;
            std::unique_ptr<epoll_event[]> _events;
            std::unique_ptr<FileEvent[]> _fileEvent;
            std::unique_ptr<ReadyEvent[]> _readyEvent;
            int wait();

            bool _quit;
    };
}
