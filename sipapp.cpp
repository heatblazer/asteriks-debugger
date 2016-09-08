#include "sipapp.h"

// pjsua //
#include <pjsua-lib/pjsua_internal.h>
#include "mem-pool.h"
// C++ //
#include <iostream>

// recorder //
#include "recorder.h"

// console //
#include "gui.h"



namespace izdebug {


void SipApp::on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rx_data)
{
    (void) rx_data;
    pjsua_call_info info;
    // fill info struct
    (void) acc_id;
    pjsua_call_get_info(call_id, &info);

    pjsua_call_answer(call_id, 200, NULL, NULL);
}

void SipApp::on_call_state(pjsua_call_id call_id, pjsip_event *ev)
{
    pjsua_call_info info;
    (void) ev;
    pjsua_call_get_info(call_id, &info);

}

void SipApp::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info info;
    pjsua_call_get_info(call_id, &info);

    if (info.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        pjsua_conf_connect(info.conf_slot, 0);
        pjsua_conf_connect(0, info.conf_slot);

        // if the player has been created
        if (1) {
            Gui::Instance().getPlayer().setSink(info.conf_slot);
            pjsua_conf_connect(Gui::Instance().getPlayer().getSlot(), info.conf_slot);
        }
    }

}

void SipApp::on_stream_created(pjsua_call_id call_id, pjmedia_stream *strm,
                               unsigned stream_idx, pjmedia_port **p_port)
{
    (void)p_port;

    std::cout << "Call id: " << call_id << std::endl;
    pjsua_stream_info info;
    pjsua_call_get_stream_info(call_id, stream_idx, &info);

    pjmedia_transport_info tinfo;
    pjsua_call_get_med_transport_info(call_id, stream_idx, &tinfo);
    pjmedia_stream_info sinfo;
    pjmedia_stream_get_info(strm, &sinfo);


    // get the conf port id
    pjsua_conf_port_id conf_id = pjsua_call_get_conf_port(call_id);

    pjsua_conf_port_info conf_info;
    pjsua_conf_get_port_info(conf_id, &conf_info);


    static char con[1024]={0};
    char name[64] = {0};
    memcpy(name, sinfo.fmt.encoding_name.ptr, sinfo.fmt.encoding_name.slen);
    sprintf(con, "type:[%d]\npayload:[%d]\nenc_name[%s]\n"
            "clock rate:[%d]\nchan_cnt[%d]\n"
            "rxlevel[%f]\ttxlevel[%f]\n"
            "samples:[%d]\tbits[%d]\n",
            sinfo.fmt.type, sinfo.fmt.pt, name,
            sinfo.fmt.clock_rate, sinfo.fmt.channel_cnt,
            conf_info.rx_level_adj, conf_info.tx_level_adj,
            conf_info.samples_per_frame, conf_info.bits_per_sample);

    Console::Instance().putData(QByteArray(con));

    // play PCMU to the port



}



SipApp::SipApp(QObject *parent)
    : QObject(parent)
{

}

SipApp::~SipApp()
{
    pjsua_destroy();
}

bool SipApp::create(const QString &uri)
{
    pj_status_t status;
    status = pjsua_create();
    if (status != PJ_SUCCESS) {
        return false;
    }

    status = pjsua_verify_url(uri.toLatin1().constData());
    if (status != PJ_SUCCESS) {
        return false;
    }

    {
        pjsua_config cfg;
        pjsua_config_default(&cfg);

        cfg.cb.on_incoming_call = &SipApp::on_incomming_call;
        cfg.cb.on_call_media_state = &SipApp::on_call_media_state;
        cfg.cb.on_call_state = &SipApp::on_call_state;
        cfg.cb.on_stream_created = &SipApp::on_stream_created;

        status = pjsua_init(&cfg, NULL, NULL);

        if (status != PJ_SUCCESS) {
            return false;
        }
    }

    {
        pjsua_transport_config cfg;
        pjsua_transport_id id;
        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &id);
        if (status != PJ_SUCCESS) {
            return false;
        }

    }

    // start pjsua
    status = pjsua_start();

    if (status != PJ_SUCCESS) {
        return false;
    }

    {
        pjsua_acc_config cfg;
        pjsua_acc_config_default(&cfg);

        char idstr[128]={0};
        sprintf(idstr, "sip:%s@%s", SIP_USER, SIP_DOMAIN);
        cfg.id = pj_str(idstr);
        cfg.reg_uri = pj_str("sip:example.com");
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(SIP_USER);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(SIP_PASS);

        status = pjsua_acc_add(&cfg, PJ_TRUE, &m_acc_id);

        if (status != PJ_SUCCESS) {
            return false;
        }

    }
    return true;
}

void SipApp::makeACall(const char* uri)
{
    char sipuri[128]={0};
    strcpy(sipuri, uri);
    pj_str_t s = pj_str(sipuri);
    pj_status_t status = pjsua_call_make_call(m_acc_id, &s
                                  , 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) {
        std::cout << "Failed to make a call!" << std::endl;
    }


}

void SipApp::hupCall()
{
    pjsua_call_hangup_all();
    //Gui::g_recorder.stop();
}

void SipApp::stopWav()
{
    pjsua_conf_disconnect(Gui::Instance().getPlayer().getSlot(),
                          Gui::Instance().getPlayer().getSink());
}

} // namespace izdebug
