#ifndef THREAD_H
#define THREAD_H

#include "defs.h"

namespace izdebug {

class MediaPort;


class Mutex
{
public:
    virtual ~Mutex() = 0;
    virtual void lock() = 0;
    virtual bool tryLock() = 0;
    virtual void unlock() = 0;
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
