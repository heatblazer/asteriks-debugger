#ifndef THREAD_H
#define THREAD_H

#include "defs.h"

#include <QMutex>

namespace izdebug {

class MediaPort;


class Mutex
{
public:
    virtual ~Mutex()=default;
    virtual void lock()=0;
    virtual bool tryLock()=0;
    virtual void unlock()=0;
};

template <class T> class LockGuard
{
public:
    explicit LockGuard(T&  m)
        : m_ref(m)
    {
        m_ref.lock();
    }

    ~LockGuard()
    {
        m_ref.unlock();
    }

private:

    T& m_ref;
};

class Thread
{

public:

    virtual ~Thread()=default;
    virtual bool create(int stack_size, int prio, thCb epoint, void* udata)=0;
    virtual void join()=0;
    virtual void resume()=0;
    virtual void sleep(unsigned)=0;
    virtual void* getCurrentThread()=0;
    virtual MediaPort* getPort()=0;

protected:
    // later refractor
};


} // namespace izdebug


#endif // THREAD_H
