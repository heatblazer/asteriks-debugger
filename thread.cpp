#include "thread.h"

#include "mediaport.h"

#include <sched.h>

namespace izdebug {

Thread::~Thread()
{

}


volatile int SpinLock::m_lock = 0;

void SpinLock::lock()
{
    while (!__sync_bool_compare_and_swap(&m_lock, 0, 1)) {
        sched_yield();
    }
}

void SpinLock::unlock()
{
    m_lock = 0; // atomic
}

MediaPort *Thread::getPort()
{
    return m_port;
}

Mutex::~Mutex()
{

}


} // namespace izdebug
