#pragma once

namespace sney
{
    class EventLoop
    {
        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void stop();

            void runInLoop();
    };
}
