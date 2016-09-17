#include "thread.h"
#include "Sip/mediaport.h"
#include <sched.h>

namespace izdebug {


Mutex::~Mutex()
{

}

QtLockGuard::QtLockGuard(QMutex &th)
    : m_ref(th)
{
    m_ref.lock();
}


QtLockGuard::~QtLockGuard()
{
    m_ref.unlock();
}

} // namespace izdebug
