#ifndef THREAD_H
#define THREAD_H

#include "defs.h"

namespace izdebug {

class MediaPort;

class SpinLock
{
public:
    static void lock();
    static void unlock();

private:
    static volatile int m_lock;
};

class Thread
{

public:
    virtual ~Thread()=0;
    virtual bool create(int stack_size, int prio, thCb epoint, void* udata)=0;
    virtual void join()=0;
    virtual void* getCurrentThread()=0;
    virtual MediaPort* getPort();

protected:
    // later refractor
    MediaPort* m_port; // aggregation
    void* usr_data;
};


} // namespace izdebug


#endif // THREAD_H
