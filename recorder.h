#ifndef RECORDER_H
#define RECORDER_H


// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

// parent //
#include "mediaport.h"
#include "Concurent/pjsua-thread.h"
#include <QTimer>
#include <QThread>

namespace izdebug {

// capture the frames to wav file,
// also debug the data
class Recorder : public MediaPort
{

public:
    // pjsuatrhead api
    static pj_status_t my_get_frame(pjmedia_port* port, pjmedia_frame* frame);
    static pj_status_t my_put_frame(pjmedia_port* port, pjmedia_frame* frame);

    static pj_status_t entryPoint(void* user_data);

    explicit Recorder(const QString& fname);
    virtual ~Recorder();
    bool create();
    bool isRecording();
    pjmedia_port* pjPort();

    // convinience
    struct {
        pjmedia_port* port;
        unsigned slot;
    } m_player; // no snd device attached


public:
    void stop();
    void start();
    void connectPlayer(unsigned src);
    void doWork(void);

private:

    bool _create(const char* fname);
    bool _create2();
    bool _create_player();


private:
    pjmedia_snd_port* p_sndPort;
    unsigned m_slot;
    bool m_isRecording;
    QString m_fname;
    int m_samples;
    pj_int16_t* p_smp_data;

    Mutex*  p_mutex;
    Thread* p_thread;
};


} // namespace izdebug


#endif // RECORDER_H
