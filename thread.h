#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <pjlib.h>

namespace izdebug {

class Thread : public QObject
{
    Q_OBJECT
public:

    enum Prio {
        LOW,
        MED,
        HIGH
    };

    static pj_status_t testCb(void* data);
    explicit Thread(QObject* parent=nullptr);
    virtual ~Thread();
    bool create(unsigned stack_size, const char* name, Prio prio,
                pj_thread_proc entry,
                void* usr_data); // dummy for now

    void join(void);

private:
    pj_thread_t*    p_thread;
    void*           p_data;
};

} // namespace izdebug


#endif // THREAD_H
