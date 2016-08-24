#include "sipapp.h"

// call //
#include "call.h"



namespace izdebug {

SipApp::SipApp(QObject *parent)
    : QObject(parent),
      p_call(nullptr)
{
    p_call = new Call(this);
}

SipApp::~SipApp()
{
    // forgot to destry pjsip and you will have problems later
    if (p_call != nullptr) {
        delete p_call;
        p_call = nullptr;
    }
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

        cfg.cb.on_incoming_call = &Call::on_incomming_call;
        cfg.cb.on_call_media_state = &Call::on_call_media_state;
        cfg.cb.on_call_state = &Call::on_call_state;

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

void SipApp::makeACall(const QString &uri)
{
    if(p_call->makeCall(m_acc_id, uri)) {
        // fill in later
    } else {
        // fill in later
    }
}

} // namespace izdebug
