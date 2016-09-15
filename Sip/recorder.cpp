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
#include "Gui/gui.h"

// mutex //
#include "Concurent/pjsua-thread.h"
#include <pjlib.h>
#include <QMutex>


static void my_put_frame(pjmedia_port* p, pjmedia_frame* frm)
{
    pjmedia_conf2* cnf = (pjmedia_conf2*) p->port_data.pdata;
    struct conf_port* port = cnf->ports[p->port_data.ldata];

    if (port->listener_cnt==0) {
        return;
    }
}

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
        r->doWork();
    }
}

Recorder::Recorder(const QString& fname)
    : MediaPort(),
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


#if 0
        pj_status_t status = pjmedia_null_port_create(Pool::Instance().toPjPool(),
                                       PJMEDIA_PIA_SRATE(&conf->info),
                                       PJMEDIA_PIA_CCNT(&conf->info),
                                       PJMEDIA_PIA_SPF(&conf->info),
                                       PJMEDIA_PIA_BITS(&conf->info),
                                       &p_port);
#endif
        pjmedia_snd_port_create(Pool::Instance().toPjPool(),
                                    -1, -1,
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

        pjmedia_conf_add_port(pjsua_var.mconf, Pool::Instance().toPjPool(),
                              p_port, NULL, &m_slot);

        m_isOk = true;

    }

    return m_isOk;
}

bool Recorder::_create2()
{


}

bool Recorder::_create_player()
{
    pj_status_t s = pjmedia_null_port_create(Pool::Instance().toPjPool(),
                                             PJMEDIA_PIA_SRATE(&p_port->info),
                                             PJMEDIA_PIA_CCNT(&p_port->info),
                                             PJMEDIA_PIA_SPF(&p_port->info),
                                             PJMEDIA_PIA_BITS(&p_port->info),
                                             &m_player.port);


    if (s != PJ_SUCCESS) {
        return false;
    }

    s = pjmedia_conf_add_port(pjsua_var.mconf, Pool::Instance().toPjPool(),
                              m_player.port, NULL, &m_player.slot);

    if (s != PJ_SUCCESS) {
        return false;
    }

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
                                     getSrc(),
                                     getSlot());

        delete p_thread;

        m_isRecording = false;

    }
    Gui::Instance().m_vuMeter.progressBar[0].setValue(0);
    Gui::Instance().m_vuMeter.progressBar[1].setValue(0);
    Gui::Instance().m_vuMeter.progressBar[2].setValue(0);

}

void Recorder::start()
{
    // start to record
    if (!m_isRecording) {
        m_isRecording = true;
        p_mutex = new PjMutex();
        p_thread = new PjThread(this);
#if 0
         pjmedia_conf_connect_port(pjsua_var.mconf, m_slot, m_sink, 0);
#endif

         pjsua_conf_connect(m_sink, m_slot);


        ((PjThread*)p_thread)->p_entry = &Recorder::entryPoint;
        p_thread->create(PJTHR_STACK_SIZE, 0, PjThread::thEntryPoint,
                         this);

    }
}

/// do specifik work here
/// hijakced conf bridge must be analyzed properly
/// \brief Recorder::doWork
///
void Recorder::doWork()
{
    if (m_isRecording) {

        unsigned int spf = PJMEDIA_PIA_SPF(&p_port->info);
        pjmedia_frame frm, frm2;
        frm.buf = Pool::Instance().zero_alloc(spf);
        pj_int16_t* smpls = new pj_int16_t[spf];
        pj_int16_t hwm=0;

        pjmedia_conf2* hijack = reinterpret_cast<pjmedia_conf2*>(pjsua_var.mconf);
        conf_port* call_port = hijack->ports[getSrc()];

        // TODO read why doubling the buffer for frame... 160 << 1???
        frm2.buf = Pool::Instance().alloc(PJMEDIA_PIA_SPF(&call_port->port->info) << 1);
        frm2.size = (PJMEDIA_PIA_SPF(&call_port->port->info)) << 1;

        // try to get a frame from the incomming port
#if 0
        pj_status_t res = pjmedia_port_get_frame(call_port->port, &frm2) ;
        if (res != PJ_SUCCESS) {
            static char txt[128]={0};
            sprintf(txt, "Failed to get frame from callID: [%d]: Err:[%d]\n",
                    getSrc(), res);
            // do we need to lock here... never missed a frame...
            Console::Instance().putData(QByteArray("Failed to get a frame"));
            return;
        }

#if 0
        pjmedia_port_put_frame(call_port->port, &frm2);
#endif


        memcpy(smpls,
               (pj_int16_t*)frm2.buf,
               spf);

        for(int i=0; i < spf; i++) {
            pj_int16_t abs_val = (pj_int16_t)abs(smpls[i]);
            if (hwm < abs_val ) {
                hwm = abs_val;
            }
        }

#endif
        unsigned tx, rx, tx2, rx2;
        pjmedia_conf_get_signal_level(pjsua_var.mconf, getSrc(), &tx, &rx);
        pjmedia_conf_get_signal_level(pjsua_var.mconf, getSlot(), &tx2, &rx2);

        pj_thread_sleep(50);

        // coming too fast from gui thread, we may corrupt the paint()
        static QMutex m;
        m.lock();
        Gui::Instance().m_vuMeter.progressBar[0].setValue(hwm);
        Gui::Instance().m_vuMeter.progressBar[1].setValue(tx);
        Gui::Instance().m_vuMeter.progressBar[2].setValue(tx2);
        m.unlock();
        QThread::currentThread()->sleep(0);


    }
}


} // namespace izdebug
