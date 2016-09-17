#ifndef PJSUATHREAD_H
#define PJSUATHREAD_H

#include "thread.h"

namespace izdebug {


class PjMutex : public Mutex
{
public:
    PjMutex();
    ~PjMutex();
    void lock();
    bool tryLock();
    void unlock();
private:

    pj_mutex_t* m_mutex;

};

class PjThread : public Thread
{
public:

    pjThrEpoint p_entry;
    static int thEntryPoint(int argc, void *argv);

public:
    explicit PjThread(MediaPort* mp);
    ~PjThread();
    bool create(int stack_size, int prio, thCb epoint, void *udata);
    void join();
    void resume();
    void sleep(unsigned ms);
    void* getCurrentThread();
    MediaPort* getPort();


private:

    pj_thread_t* p_thr;
    bool m_isRunning;
    MediaPort* m_port; // aggregation
    void* usr_data;

};


class PjLockGuard
{
public:
    explicit PjLockGuard(PjMutex* const m) ;
    ~PjLockGuard();
private:
    PjMutex* const m_mutex;

};


} // namespace izdebug

#endif // PJSUATHREAD_H
