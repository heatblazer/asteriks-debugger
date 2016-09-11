#include "thread.h"

#include "mediaport.h"


namespace izdebug {

Thread::Thread(MediaPort* mp, QObject *parent)
    : QThread(parent)
{
    m_port = mp;
}

Thread::~Thread()
{

}

void Thread::run()
{
    m_port->doWork(NULL);
}

} // namespace izdebug
