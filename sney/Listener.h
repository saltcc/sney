#pragma once
#include <memory>
#include <noncopyable.h>

namespace sney
{
    class EventLoop;

    class Listener : private noncopyable
    {
        public:
            typedef std::function< void(int sfd, const std::string&) > NewConnCallback;

            Listener(EventLoop *loop,  const std::string &addr);
            ~Listener();

            void Accept();
            void Stop();
            void SetNewConnCallback(NewConnCallback cb)
            {
                _newConnCb = cb;
            }
            
        private:

            int _sfd;
            EventLoop *_loop;
            NewConnCallback _newConnCb;
    };
}
