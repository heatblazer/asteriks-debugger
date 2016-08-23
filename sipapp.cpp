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

    pjsua_config_default(&m_config);

    m_config.cb.on_incoming_call = &Call::on_incomming_call;
    m_config.cb.on_call_media_state = &Call::on_call_media_state;
    m_config.cb.on_call_state = &Call::on_call_state;

    status = pjsua_init(&m_config, NULL, NULL);

    if (status != PJ_SUCCESS) {
        return false;
    }

    pjsua_transport_config_default(&m_transCfg);

    m_transCfg.port = 5060;

    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &m_transCfg, NULL);
    if (status != PJ_SUCCESS) {
        return false;
    }

    pjsua_acc_config_default(&m_accCfg);

    m_accCfg.id = pj_str("sip:"SIP_DOMAIN);
    m_accCfg.reg_uri = pj_str("sip:"SIP_DOMAIN);
    m_accCfg.cred_count = 1;
    m_accCfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
    m_accCfg.cred_info[0].scheme = pj_str("digest");
    m_accCfg.cred_info[0].username = pj_str(SIP_USER);
    m_accCfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    m_accCfg.cred_info[0].data = pj_str(SIP_PASS);

    status = pjsua_acc_add(&m_accCfg, PJ_TRUE, &m_acc_id);

    if (status != PJ_SUCCESS) {
        return false;
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
