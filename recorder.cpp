#include "recorder.h"

// pjlib //
#include "pjsua-lib/pjsua_internal.h"
#include <pjsua.h>

// local //
#include "mem-pool.h"

// ansi C //
#include <stdio.h>

// console //
#include "gui.h"

namespace izdebug {


int Recorder::latency(void *usr)
{
    Recorder* r = (Recorder*) usr;
    pjmedia_port* p_port = r->p_port;
    static pj_mutex_t *m;
    pj_mutex_create(Pool::Instance().toPjPool(), NULL, 0, &m);
    for(;;) {
        pj_mutex_lock(m);
        pj_thread_sleep(500);
        pjmedia_frame frame;
        //pjmedia_port* p_port = pjmedia_conf_get_master_port(pjsua_var.mconf);
        short* buf;
        unsigned i, samples_per_frame;
        pj_size_t read, len;
        unsigned start_pos;
        unsigned lat_sum = 0,
                lat_cnt = 0,
                lat_min = 10000,
                lat_max = 0;

        samples_per_frame = PJMEDIA_PIA_SPF(&p_port->info);

        frame.buf = pj_pool_alloc(Pool::Instance().toPjPool(), samples_per_frame);
        frame.size = samples_per_frame;

        len = samples_per_frame;
        buf = (short*)pj_pool_alloc(Pool::Instance().toPjPool(), samples_per_frame);

        read = 0;
        while (read < len) {
            pjmedia_port_get_frame(p_port, &frame);
            pjmedia_copy_samples(buf, (short*)frame.buf, samples_per_frame);

            read += samples_per_frame; // advance
        }

        start_pos = 0;
        unsigned srate = PJMEDIA_PIA_SRATE(&p_port->info);
        while (start_pos < srate) {
            int max_signal = 0;
            unsigned max_signal_pos = start_pos;
            unsigned max_echo_pos = 0;
            unsigned pos;
            unsigned lat;

            // get the largest signal
            for(int i=start_pos; i < start_pos + PJMEDIA_PIA_SRATE(&p_port->info); ++i) {
                if (abs(buf[i]) > max_signal) {
                    max_signal = abs(buf[i]);
                    max_signal_pos = i;
                }
            }

            pos = max_signal_pos + 10 * PJMEDIA_PIA_SRATE(&p_port->info) / 1000;
            max_signal = 0;
            max_echo_pos = pos;

            for(int i=pos; i < pos + PJMEDIA_PIA_SRATE(&p_port->info)/2; ++i) {
                if (abs(buf[i]) > max_signal) {
                    max_signal = abs(buf[i]);
                    max_echo_pos = i;
                }
            }

            lat = (max_echo_pos - max_signal_pos) * 1000 / PJMEDIA_PIA_SRATE(&p_port->info);
            char txt[64]={0};
            printf( "Latency = %u\n", lat);
       //     Console::Instance().putData(txt);

            lat_sum += lat;
            lat_cnt++;

            if (lat < lat_min) {
                lat_min = lat;
            }
            if (lat > lat_max) {
                lat_max = lat;
            }

            start_pos += PJMEDIA_PIA_SRATE(&p_port->info);

        }

        char txt[256]={0};

        printf( "Latency avg = %u\n"
                "Latency minimum = %u\n"
                "Latency maximum = %u\n"
                "Number of data = %u\n",
                lat_sum/lat_cnt, lat_min, lat_max, lat_cnt);
    //    Console::Instance().putData(txt);
    }

}

Recorder::Recorder(const QString& fname, QObject *parent)
    : MediaPort(parent),
      p_port(nullptr),
      p_sndPort(nullptr),
      m_slot(0),
      m_isOk(false),
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

bool Recorder::_create(const char *fname)
{
    if (!m_isOk) {
        pjmedia_port* conf = pjmedia_conf_get_master_port(pjsua_var.mconf);

        m_samples = PJMEDIA_PIA_SPF(&conf->info);

        if (conf == NULL) {
            // no conf bridge? why?
        }

        pjmedia_snd_port_create_rec(Pool::Instance().toPjPool(),
                                    -1,
                                    PJMEDIA_PIA_SRATE(&conf->info),
                                    PJMEDIA_PIA_CCNT(&conf->info),
                                    PJMEDIA_PIA_SPF(&conf->info),
                                    PJMEDIA_PIA_BITS(&conf->info),
                                    0,
                                    &p_sndPort);

        pj_status_t status = pjmedia_wav_writer_port_create(Pool::Instance().toPjPool(),
                                       "caputre_pcma.wav",
                                       PJMEDIA_PIA_SRATE(&conf->info),
                                       PJMEDIA_PIA_CCNT(&conf->info),
                                       PJMEDIA_PIA_SPF(&conf->info),
                                       PJMEDIA_PIA_BITS(&conf->info),
                                       0, 0,
                                       &p_port);


         {

//            pjmedia_snd_port_connect(p_sndPort, p_port);
            status = pjmedia_conf_add_port(pjsua_var.mconf,
                                       Pool::Instance().toPjPool(),
                                       p_port,
                                       NULL,
                                       &m_slot);
            if (status != PJ_SUCCESS) {
                pjmedia_port_destroy(p_port);
                return false;
            }

        }
        m_isOk = true;
        m_timer.setInterval(1000);
        connect(this, SIGNAL(recording(bool)),
                this, SLOT(hRec(bool)));
        connect(&m_timer, SIGNAL(timeout()),
                this, SLOT(hTimeout2()), Qt::DirectConnection);
        return true;
    }

    return false;
}

void Recorder::destroy()
{
    _disconnect_and_remove();
}


pjmedia_port *Recorder::pjPort()
{
    return p_port;
}

void Recorder::stop()
{
    if (m_isRecording) {
        //_disconnect_and_remove();
        //m_timer.stop();

        m_isRecording = false;
    }
    emit recording(false);
}

void Recorder::start()
{
    // start to record
    if (!m_isRecording) {
        m_timer.start();
        m_isRecording = true;
        //m_thread.create(0, NULL, Thread::MED, Recorder::latency, this);

     }
    emit recording(true);

}



void Recorder::hRec(bool status)
{
    if (status) {
        pj_status_t s1 = pjmedia_conf_connect_port(pjsua_var.mconf, 0, m_slot, 0);
   //     pj_status_t s2 = pjmedia_conf_connect_port(pjsua_var.mconf, m_slot, 0, 0);
        if (s1 == PJ_SUCCESS) {
            //
        }

     } else {
        pjmedia_conf_disconnect_port(pjsua_var.mconf, 0, m_slot);
     //   pjmedia_conf_disconnect_port(pjsua_var.mconf, m_slot, 0);

        return;
    }


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

void Recorder::hTimeout2()
{

    pjmedia_frame frame;
    //pjmedia_port* p_port = pjmedia_conf_get_master_port(pjsua_var.mconf);
    short* buf;
    unsigned i, samples_per_frame;
    pj_size_t read, len;
    unsigned start_pos;
    unsigned lat_sum = 0,
            lat_cnt = 0,
            lat_min = 10000,
            lat_max = 0;

    samples_per_frame = PJMEDIA_PIA_SPF(&p_port->info);

    frame.buf = pj_pool_alloc(Pool::Instance().toPjPool(), samples_per_frame);
    frame.size = samples_per_frame;

    len = samples_per_frame;
    buf = (short*)pj_pool_alloc(Pool::Instance().toPjPool(), samples_per_frame);

    read = 0;
    while (read < len) {
        pjmedia_port_get_frame(p_port, &frame);
        pjmedia_copy_samples(buf, (short*)frame.buf, samples_per_frame);

        read += samples_per_frame; // advance
    }

    start_pos = 0;
    unsigned srate = PJMEDIA_PIA_SRATE(&p_port->info);
    while (start_pos < srate) {
        int max_signal = 0;
        unsigned max_signal_pos = start_pos;
        unsigned max_echo_pos = 0;
        unsigned pos;
        unsigned lat;

        // get the largest signal
        for(int i=start_pos; i < start_pos + PJMEDIA_PIA_SRATE(&p_port->info); ++i) {
            if (abs(buf[i]) > max_signal) {
                max_signal = abs(buf[i]);
                max_signal_pos = i;
            }
        }

        pos = max_signal_pos + 10 * PJMEDIA_PIA_SRATE(&p_port->info) / 1000;
        max_signal = 0;
        max_echo_pos = pos;

        for(int i=pos; i < pos + PJMEDIA_PIA_SRATE(&p_port->info)/2; ++i) {
            if (abs(buf[i]) > max_signal) {
                max_signal = abs(buf[i]);
                max_echo_pos = i;
            }
        }

        lat = (max_echo_pos - max_signal_pos) * 1000 / PJMEDIA_PIA_SRATE(&p_port->info);
        char txt[64]={0};
        printf( "Latency = %u\n", lat);
   //     Console::Instance().putData(txt);

        lat_sum += lat;
        lat_cnt++;

        if (lat < lat_min) {
            lat_min = lat;
        }
        if (lat > lat_max) {
            lat_max = lat;
        }

        start_pos += PJMEDIA_PIA_SRATE(&p_port->info);

    }

    char txt[256]={0};

    printf( "Latency avg = %u\n"
            "Latency minimum = %u\n"
            "Latency maximum = %u\n"
            "Number of data = %u\n",
            lat_sum/lat_cnt, lat_min, lat_max, lat_cnt);
//    Console::Instance().putData(txt);

}

void Recorder::_disconnect_and_remove()
{
    if (m_isRecording && m_isOk) {
        pj_status_t status = pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     0, m_slot);

        if (status == PJ_SUCCESS) {
            pjmedia_port_destroy(p_port);
            m_isOk = false; // we can create it again
        }
    }
}

} // namespace izdebug
