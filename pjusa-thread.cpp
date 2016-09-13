#include "pjsua-thread.h"

#include <pjlib.h>

#include <iostream>

#include <pjlib.h>
// memory //
#include "mem-pool.h"

namespace izdebug {

PjMutex::~PjMutex()
{
    pj_mutex_destroy(m_mutex);
}

PjMutex::PjMutex()
{
    pj_mutex_create(Pool::Instance().toPjPool(), NULL, PJ_MUTEX_DEFAULT, &m_mutex);
}

void PjMutex::lock()
{
    pj_mutex_lock(m_mutex);
}

void PjMutex::unlock()
{
    pj_mutex_unlock(m_mutex);
}

bool PjMutex::tryLock()
{
    if (pj_mutex_trylock(m_mutex) != PJ_SUCCESS) {
        return false;
    }
    return true;
}

/// thread proxy function
/// \brief PjThread::thEntryPoint
/// \param argc
/// \param argv
/// \return
///
int PjThread::thEntryPoint(int argc, void *argv)
{
    (void) argc;
    PjThread* p = (PjThread*) argv;
    void* data = ((PjThread*)p)->usr_data;
    ((PjThread*)p)->p_entry(data);
    ((PjThread*)p)->m_isRunning = true;
}

PjThread::PjThread(MediaPort *mp)
    :p_thr(NULL),
    m_isRunning(false),
    usr_data(NULL)
{
    m_port = mp;
}



PjThread::~PjThread()
{
}


bool PjThread::create(int stack_size, int prio, thCb epoint, void *udata)
{
    pj_status_t status = pj_thread_create(Pool::Instance().toPjPool(),
                                          NULL, p_entry, this,
                                          (pj_ssize_t)stack_size,
                                          0, &p_thr);
    if (status != PJ_SUCCESS) {
        return false;
    }
    // set priorties latter

    return true;
}

void PjThread::join()
{
    pj_status_t status = pj_thread_join(p_thr);
    if (status != PJ_SUCCESS) {
        //
    }
}

void *PjThread::getCurrentThread()
{
    pj_thread_t* self = pj_thread_this();
    return (pj_thread_t*)self;
}


} // namespace izdebug

