#include "recorder.h"

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


namespace izdebug {

Recorder::Recorder(const QString& fname, QObject *parent)
    : MediaPort(parent),
      p_sndPort(nullptr),
      m_slot(0),
      m_isRecording(false),
      m_fname(fname),
      m_samples(0)
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
    return _create(m_fname.toLatin1().constData());
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


            status = pjmedia_conf_add_port(pjsua_var.mconf,
                                       Pool::Instance().toPjPool(),
                                       p_port,
                                       NULL,
                                       &m_slot);
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

pjmedia_port *Recorder::pjPort()
{
    return p_port;
}

void Recorder::stop()
{
    if (m_isRecording) {
        //_disconnect_and_remove();
        m_isRecording = false;
    }
    emit recording(false);
}

void Recorder::start()
{
    // start to record
    if (!m_isRecording) {
         m_isRecording = true;
    }
    emit recording(true);

}


void Recorder::hTimeout()
{
    pjmedia_frame frame;
    pj_int16_t framebuf[m_samples];
    unsigned ms;

    if(1){
        pjmedia_port_get_frame(p_port, &frame);

        union {
            pj_int32_t i;
            char c[sizeof(pj_int32_t)];
        } level32;
        level32.i = pjmedia_calc_avg_signal(framebuf,
                          PJMEDIA_PIA_SPF(&p_port->info));
        union {
            int i;
            char c[sizeof(int)];
        } level;

        level.i = pjmedia_linear2ulaw(level32.i) ^ 0xFF;
        unsigned tx, rx;
        pjmedia_conf_get_signal_level(pjsua_var.mconf, m_slot, &tx, &rx);
        char txt[32]={0};
        sprintf(txt, "tx:[%d]\trx[%d]\n", tx, rx);
        Console::Instance().putData(txt);

        for(int i=0; i < 8; ++i) {
            ms = i * 1000 * PJMEDIA_PIA_SPF(&p_port->info) /
                PJMEDIA_PIA_SRATE(&p_port->info);
            char txt[128]={0};
            sprintf(txt, "%03d.%03d\t%7d\t%7d\n",
                   ms/1000, ms%1000, level.i, level32.i);
            Console::Instance().putData(txt);
        }
    }
}


void Recorder::hTimeout3()
{
    std::cout << "TIMEOIT RECORDER" << std::endl;
    pjmedia_frame frame;

    pj_int16_t* framebuf = new pj_int16_t[m_samples];

    if(framebuf==NULL) {
        return;
    }

    unsigned ms=0;

    pjmedia_port_get_frame(p_port, &frame);

    union {
        pj_int32_t i;
        char c[sizeof(pj_int32_t)];
    } level32;

    union {
        int i;
        char c[sizeof(int)];
    } level;

    level32.i = pjmedia_calc_avg_signal(framebuf,
                                        PJMEDIA_PIA_SPF(&p_port->info));

    level.i = pjmedia_linear2ulaw(level32.i) ^ 0xFF; // toggle
    unsigned tx, rx;
    pjmedia_conf_get_signal_level(pjsua_var.mconf, m_slot, &tx, &rx);

    emit sendRxTx(tx, rx);
}

void Recorder::_disconnect_and_remove()
{
    if (m_isRecording && m_isOk) {
        pj_status_t status = pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     0, m_slot);

        if (status == PJ_SUCCESS) {
            m_isOk = false;
        }
    }
}

} // namespace izdebug
