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

    PjThread(MediaPort* mp);
    ~PjThread();
    bool create(int stack_size, int prio, thCb epoint, void *udata);
    void join();
    void* getCurrentThread();

private:

    pj_thread_t* p_thr;
    bool m_isRunning;
    void* usr_data;
};

} // namespace izdebug

#endif // PJSUATHREAD_H
