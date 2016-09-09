#ifndef RECORDER_H
#define RECORDER_H

// qt //
#include <QTimer>

// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

// parent //
#include "mediaport.h"

namespace izdebug {

// capture the frames to wav file,
// also debug the data
class Recorder : public MediaPort
{
    Q_OBJECT
public:
    static int entryPoint(void* user_data);
    static int latency(void* usr);

    explicit Recorder(const QString& fname, QObject* parent=nullptr);
    virtual ~Recorder();
    bool create();
    bool isRecording();
    pjmedia_port* pjPort();

signals:
    void sendFrame(pjmedia_frame* frm);
    void recording(bool);
    void sendRxTx(unsigned tx, unsigned rx);

//public slots:
public:
    void stop();
    void start();

public slots:
    void hTimeout(void);
    void hTimeout3(void);

private:
    void _disconnect_and_remove();
    bool _create(const char* fname);


private:
    pjmedia_snd_port* p_sndPort;
    unsigned m_slot;
    bool m_isRecording;
    QString m_fname;
    int m_samples;

};


} // namespace izdebug


#endif // RECORDER_H
