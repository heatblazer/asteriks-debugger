#ifndef THREAD_H
#define THREAD_H

#include <QThread>

namespace izdebug {

class MediaPort;

class Thread : public QThread
{

public:
    explicit Thread(MediaPort *mp, QObject* parent=nullptr);
    virtual ~Thread();

    void run();

private:
    MediaPort* m_port; // aggregation
};


} // namespace izdebug


#endif // THREAD_H
