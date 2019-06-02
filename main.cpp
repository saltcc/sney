#include <iostream>

#include <Mutex.h>
#include <Condition.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <EpollEvent.h>

static int count = 0;

sney::Mutex mt;
sney::Condition cond(mt);

void safe_increment()
{
    while (1)
    {
        sney::LockGuard lg(mt);

        count++;

        if (count >= 100)
        {
            std::cout<<"mmm  "<<count<<std::endl;
            cond.notify();
        }
    }
}

void safe_print()
{
    while(1)
    {
        sney::LockGuard lg(mt);       

        while (count < 100)
        {
            std::cout<<"1111  "<<count<<std::endl;
            cond.wait();
            std::cout<<"zzzz  "<<count<<std::endl;
        }

        std::cout<<"count : "<<count<<std::endl;

        count = 0;
    }
}

int main()
{
    sney::Mutex mt;
    sney::LockGuard ml(mt);

    ///std::thread t1(safe_increment);
    //std::thread t2(safe_print);

    //sleep(1000);
    sney::EpollEvent e;
    e.epollEventInit(10);

    return 0;
}
