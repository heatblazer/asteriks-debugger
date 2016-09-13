#include "thread.h"
#include "mediaport.h"
#include <sched.h>

namespace izdebug {

Thread::~Thread()
{

}


MediaPort *Thread::getPort()
{
    return m_port;
}

Mutex::~Mutex()
{

}


} // namespace izdebug
