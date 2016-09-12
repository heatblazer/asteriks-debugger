#include "recorder.h"
#define PJTHR_STACK_SIZE 128*1024
// pjlib //
#include "pjsua-lib/pjsua_internal.h"
#include <pjsua.h>

// local //
#include "mem-pool.h"

// ansi C //
#include <stdio.h>

// C++ //
#include <iostream>

// console //
#include "gui.h"

// mutex //
#include "pjsua-thread.h"
#include <pjlib.h>
#include <QMutex>

namespace izdebug {

pj_status_t Recorder::my_get_frame(pjmedia_port *port, pjmedia_frame *frame)
{
    if(port->port_data.pdata != NULL) {
        pj_int16_t* smp = new pj_int16_t[port->port_data.ldata];
        memcpy(smp, (pj_int16_t*)port->port_data.pdata, port->port_data.ldata);

    }
}

pj_status_t Recorder::my_put_frame(pjmedia_port *port, pjmedia_frame *frame)
{

    if(frame->buf) {
        pj_int16_t* smp = new pj_int16_t[frame->size];
        memcpy(smp, (pj_int16_t*)frame->buf, frame->size);

    }
}

pj_status_t Recorder::entryPoint(void *user_data)
{


    PjThread* t = (PjThread*) user_data;
    Recorder* r = (Recorder*)t->getPort();
    for(;;)
    {
        r->hTimeout3();
    }
}

Recorder::Recorder(const QString& fname, QObject *parent)
    : MediaPort(parent),
      p_sndPort(nullptr),
      m_slot(0),
      m_isRecording(false),
      m_fname(fname),
      m_samples(0),
      p_smp_data(nullptr),
      p_thread(nullptr)
{

    // all was ok ...

}

Recorder::~Recorder()
{
    if (m_isOk) {
        _disconnect_and_remove();
    }
}

bool Recorder::create()
{
//    return _create(m_fname.toLatin1().constData());
    if (!m_isOk) {
        return _create("recorder.wav");
    }
}

bool Recorder::isRecording()
{
    return m_isRecording;
}

bool Recorder::_create(const char *fname)
{
    if (!m_isOk) {
        pjmedia_port* conf = pjmedia_conf_get_master_port(pjsua_var.mconf);

        m_samples = PJMEDIA_PIA_SPF(&conf->info);

        if (conf == NULL) {
            // no conf bridge? why?
        }

        pj_status_t status = pjmedia_wav_writer_port_create(Pool::Instance().toPjPool(),
                                       "test_wave_rec.wav",
                                       PJMEDIA_PIA_SRATE(&conf->info),
                                       PJMEDIA_PIA_CCNT(&conf->info),
                                       PJMEDIA_PIA_SPF(&conf->info),
                                       PJMEDIA_PIA_BITS(&conf->info),
                                       0, 0,
                                       &p_port);
        pjmedia_snd_port_create_rec(Pool::Instance().toPjPool(),
                                    -1,
                                    PJMEDIA_PIA_SRATE(&conf->info),
                                    PJMEDIA_PIA_CCNT(&conf->info),
                                    PJMEDIA_PIA_SPF(&conf->info),
                                    PJMEDIA_PIA_BITS(&conf->info),
                                    0,
                                    &p_sndPort);


         {



            if (status != PJ_SUCCESS) {
                pjmedia_port_destroy(p_port);
                return false;
            }

            pjmedia_snd_port_connect(p_sndPort, p_port);

        }

        m_isOk = true;
        return true;
    }

    return false;
}

bool Recorder::_create2()
{

    p_port = (pjmedia_port*)Pool::Instance().zero_alloc(sizeof(pjmedia_port));
    if(!p_port) {
        return false;
    }

    pjmedia_port* p = pjmedia_conf_get_master_port(pjsua_var.mconf);

    pj_str_t name = pj_str("special port");
    pjmedia_port_info_init(&p_port->info,
                           &name,
                           PJMEDIA_SIG_CLASS_PORT_AUD('s', 'i'),
                           PJMEDIA_PIA_SRATE(&p->info),
                           PJMEDIA_PIA_CCNT(&p->info),
                           PJMEDIA_PIA_BITS(&p->info),
                           PJMEDIA_PIA_SPF(&p->info)
                           );


    p_port->get_frame = &Recorder::my_get_frame;
    p_port->put_frame = &Recorder::my_put_frame;


    p_smp_data = (pj_int16_t*)Pool::Instance().zero_alloc(PJMEDIA_PIA_SPF(&p_port->info));


    pjmedia_snd_port_create(Pool::Instance().toPjPool(),
                                -1, -1,
                                PJMEDIA_PIA_SRATE(&p->info),
                                PJMEDIA_PIA_CCNT(&p->info),
                                PJMEDIA_PIA_SPF(&p->info),
                                PJMEDIA_PIA_BITS(&p->info),
                                0,
                                &p_sndPort);

    pjmedia_snd_port_connect(p_sndPort, p_port);

    return true;

}


pjmedia_port *Recorder::pjPort()
{
    return p_port;
}

void Recorder::stop()
{
    if (m_isRecording) {
        //_disconnect_and_remove();
        pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     getSlot(),
                                     getSink());
        m_isRecording = false;

    }
    emit recording(false);
}

void Recorder::start()
{
    // start to record
    if (!m_isRecording) {
        m_isRecording = true;
        p_thread = new PjThread(this);
        pjmedia_conf_add_port(pjsua_var.mconf, Pool::Instance().toPjPool(),
                              p_port, NULL, &m_slot);
        pjsua_conf_connect(getSink(),
                           getSlot());

        ((PjThread*)p_thread)->p_entry = &Recorder::entryPoint;
        p_thread->create(PJTHR_STACK_SIZE, 0, PjThread::thEntryPoint,
                         this);


    }
    emit recording(true);
}


void Recorder::hTimeout3()
{
    if (m_isRecording) {

        pjmedia_frame frame, frame2;

        pj_int16_t* framebuf = new pj_int16_t[m_samples];
        pj_int32_t level32 = pjmedia_calc_avg_signal(framebuf,
                                             PJMEDIA_PIA_SPF(&p_port->info));
        int level = pjmedia_linear2ulaw(level32) ^ 0xFF; // toggle

        unsigned tx, rx;
        pjmedia_conf_get_signal_level(pjsua_var.mconf, getSlot(), &tx, &rx);

        pj_thread_sleep(50);

        Gui::Instance().m_vuMeter.progressBar[0].setValue(tx);
        Gui::Instance().m_vuMeter.progressBar[1].setValue(rx);
    }

}

void Recorder::_disconnect_and_remove()
{

}

} // namespace izdebug
