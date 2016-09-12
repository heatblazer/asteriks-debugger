#include "pjsua-thread.h"

#include <pjlib.h>

#include <iostream>

// memory //
#include "mem-pool.h"

namespace izdebug {

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

