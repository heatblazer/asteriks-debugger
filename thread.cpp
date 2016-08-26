#include "thread.h"

// console //
#include "gui.h"

// ansi C //
#include <iostream>
// mem //
#include "mem-pool.h"
namespace izdebug {

pj_status_t Thread::testCb(void *data)
{
    for(;;) {
        std::cout << "Running..." << std::endl;
        pj_thread_sleep(100);
    }
}

Thread::Thread(QObject *parent)
    : QObject(parent)
{

}

Thread::~Thread()
{

}

bool Thread::create(unsigned stack_size, const char *name, Thread::Prio prio, pj_thread_proc entry, void *usr_data)
{
    if (stack_size == 0) {
        stack_size = 128 * 1024;
    }

    pj_status_t status = pj_thread_create(Pool::Instance().toPjPool(),
                     name, entry, usr_data, stack_size, 0, &p_thread);

    if (status != PJ_SUCCESS) {
        return false;
    }

}

void Thread::join()
{
    pj_thread_join(p_thread);

}

} // namespace izdebug

