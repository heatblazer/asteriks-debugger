#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>

// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

// thr //
#include "thread.h"

namespace izdebug {

// capture the frames to wav file,
// also debug the data
class Recorder : public QObject
{
    Q_OBJECT
public:
    static int entryPoint(void* user_data);

    explicit Recorder(QObject* parent=nullptr);
    virtual ~Recorder();
    bool create(const char* fname);
    void destroy(void);
    pjmedia_port* pjPort();

signals:
    void sendFrame(pjmedia_frame* frm);

public slots:
    void stop();
    void start();
    void start2();


private:
    void _disconnect_and_remove();

private:

    pjmedia_port* p_port;
    unsigned m_slot;
    bool m_isOk;
    bool m_isRecording;

    Thread m_thread;

};


} // namespace izdebug


#endif // RECORDER_H
