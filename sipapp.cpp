#include "sipapp.h"

// call //
#include "call.h"

// C++ //
#include <iostream>

namespace izdebug {


void SipApp::on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rx_data)
{
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


    //emit hasMessage(m_message);

}

void SipApp::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info info;
    pjsua_call_get_info(call_id, &info);

    if (info.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        pjsua_conf_connect(info.conf_slot, 0);
        pjsua_conf_connect(0, info.conf_slot);
    }

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

        cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
        cfg.reg_uri = pj_str("sip:"SIP_DOMAIN);
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
    pj_str_t s = pj_str((char*)uri);
    pj_status_t status = pjsua_call_make_call(m_acc_id, &s
                                  , 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) {
        std::cout << "Failed to make a call!" << std::endl;
    }
}

void SipApp::hupCall()
{
    pjsua_call_hangup_all();
}

} // namespace izdebug
