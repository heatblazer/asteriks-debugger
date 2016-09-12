#ifndef THREAD_H
#define THREAD_H

#include "defs.h"

namespace izdebug {

class MediaPort;

class Thread
{
public:
    static int thEntryPoint(int argc, void* argv);

public:
    virtual ~Thread()=0;
    virtual bool create(int stack_size, int prio, thCb epoint, void* udata)=0;
    virtual void join()=0;
    virtual void* getCurrentThread()=0;


protected:
    // later refractor
    MediaPort* m_port; // aggregation
    void* usr_data;
};


} // namespace izdebug


#endif // THREAD_H
