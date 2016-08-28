#ifndef RECORDER_H
#define RECORDER_H

// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

// parent //
#include "mediaport.h"
// thr //
#include "thread.h"

namespace izdebug {

// capture the frames to wav file,
// also debug the data
class Recorder : public MediaPort
{
    Q_OBJECT
public:
    static int entryPoint(void* user_data);

    explicit Recorder(const QString& fname, QObject* parent=nullptr);
    virtual ~Recorder();
    bool create();
    void destroy(void);
    pjmedia_port* pjPort();

signals:
    void sendFrame(pjmedia_frame* frm);
    void recording(bool);

public slots:
    void stop();
    void start();

private slots:
    void hRec(bool status);

private:
    void _disconnect_and_remove();
    bool _create(const char* fname);


private:
    pjmedia_port* p_port;
    unsigned m_slot;
    bool m_isOk;
    bool m_isRecording;
    QString m_fname;
    Thread m_thread;

};


} // namespace izdebug


#endif // RECORDER_H
