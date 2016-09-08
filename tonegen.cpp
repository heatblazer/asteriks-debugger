#include "tonegen.h"

#include "defs.h"
#include "mem-pool.h" // my mem pool
#include <pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

namespace izdebug {

pj_status_t ToneGen::cbGetFrame(pjmedia_port *port, pjmedia_frame *frm)
{
    pj_int16_t* data = (pj_int16_t*)port->port_data.pdata;
    pj_int16_t* samples = (pj_int16_t*)frm->buf;
    pj_size_t count = frm->size / 2 / PJMEDIA_PIA_CCNT(&port->info);

    unsigned int i, left=0, right=0;

    for(i=0; i < count; i++) {
        *samples++ = data[left++];

        if(PJMEDIA_PIA_CCNT(&port->info)==2) {
            *samples++ = data[right];
            right += 2;
            right = (right >= count) ? right : 0;
        }
    }
    frm->type = PJMEDIA_FRAME_TYPE_AUDIO;
    return PJ_SUCCESS;
}

ToneGen::ToneGen(unsigned srate, unsigned chans, QObject *parent)
    : MediaPort(parent),
      p_sndPort(nullptr),
      m_srate(srate),
      m_chans(chans)
{

}

ToneGen::~ToneGen()
{

}

bool ToneGen::create()
{
    return _create_sine_port(m_srate, m_chans);
}

bool ToneGen::_create_sine_port(unsigned sampling_rate, unsigned channel_count)
{
    if (!m_isOk) {
        pjmedia_port* conf = pjmedia_conf_get_master_port(pjsua_var.mconf);

        if (conf == NULL) {
            // no conf bridge? why?
        }

        pjmedia_snd_port_create_player(Pool::Instance().toPjPool(),
                                    -1,
                                    PJMEDIA_PIA_SRATE(&conf->info),
                                    PJMEDIA_PIA_CCNT(&conf->info),
                                    PJMEDIA_PIA_SPF(&conf->info),
                                    PJMEDIA_PIA_BITS(&conf->info),
                                    0,
                                    &p_sndPort);


        {
            pj_str_t name;
            p_port = (pjmedia_port*)Pool::Instance().zero_alloc(sizeof(pjmedia_port));
            if(p_port==NULL) {
                return m_isOk;
            }
            pjmedia_port_info_init(&p_port->info, &name,
                                   PJMEDIA_SIG_CLASS_PORT_AUD('s', 'i'),
                                   m_srate,
                                   m_chans,
                                   16, m_srate * 20 / 1000 * m_chans);

            p_port->get_frame = &cbGetFrame;
            pj_int16_t* data=NULL;

            p_port->port_data.pdata = (pj_int16_t*) Pool::Instance().zero_alloc(sizeof(pj_int16_t));
            unsigned count = PJMEDIA_PIA_SPF(&p_port->info) / m_chans;
            data = (pj_int16_t*) Pool::Instance().zero_alloc(count*sizeof(pj_int16_t));
            if(!data) {
                return m_isOk;
            }

            for(int i=0; i < count; i++) {

                data[i] = (pj_int16_t) (10000.0 *
                                              sin(((double)i / (double)count)) *
                                            MATH_PI * 8);

            }
        }

        {
//            pjmedia_snd_port_connect(p_sndPort, p_port);
            pj_status_t status = pjmedia_conf_add_port(pjsua_var.mconf,
                                       Pool::Instance().toPjPool(),
                                       p_port,
                                       NULL,
                                       &m_slot);
            if (status != PJ_SUCCESS) {
                pjmedia_port_destroy(p_port);
                return m_isOk;
            }

        }
        m_isOk = true;
    }

    return m_isOk;
}

} // namespace izdebug
