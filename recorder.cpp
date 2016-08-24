#include "recorder.h"

// pjlib //
#include "pjsua-lib/pjsua_internal.h"
#include <pjsua.h>

// local //
#include "mem-pool.h"

// ansi C //
#include <stdio.h>


namespace izdebug {

Recorder::Recorder()
    : p_port(nullptr),
      m_id(-1)
{

}

Recorder::~Recorder()
{
    pjmedia_port_destroy(p_port);
    m_id = -1;
}

bool Recorder::create(const char *fname, bool is_stream)
{
    pjmedia_port* conf = pjmedia_conf_get_master_port(pjsua_var.mconf);
    if (conf != NULL) {

        pj_status_t status = pjmedia_wav_writer_port_create(
                    Pool::Instance().toPjPool(), fname,
                    PJMEDIA_PIA_SRATE(&conf->info),
                    PJMEDIA_PIA_CCNT(&conf->info),
                    PJMEDIA_PIA_SPF(&conf->info),
                    PJMEDIA_PIA_BITS(&conf->info),
                    0,
                    0,
                    &p_port);

        if (status != PJ_SUCCESS) {
            return false;
        }


        unsigned slot = 0;
        status = pjmedia_conf_add_port(pjsua_var.mconf,
                                       Pool::Instance().toPjPool(),
                                       p_port, NULL, &slot);
        if (status != PJ_SUCCESS) {
            return false;
        }

        status = pjmedia_conf_connect_port(pjsua_var.mconf,
                                           0, slot, 0);

        status = pjmedia_conf_connect_port(pjsua_var.mconf,
                                           slot, 0, 0);


        // finaly return
        return true;
    }

    return false;
}

pjmedia_port *Recorder::pjPort()
{
    return p_port;
}

} // namespace izdebug

