#include "call.h"

namespace izdebug {

void Call::on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rx_data)
{
    pjsua_call_info info;
    // fill info struct
    (void) acc_id;
    pjsua_call_get_info(call_id, &info);

    pjsua_call_answer(call_id, 200, NULL, NULL);
}

void Call::on_call_state(pjsua_call_id call_id, pjsip_event *ev)
{
    pjsua_call_info info;
    (void) ev;
    pjsua_call_get_info(call_id, &info);


    //emit hasMessage(m_message);

}

void Call::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info info;
    pjsua_call_get_info(call_id, &info);

    if (info.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        pjsua_conf_connect(info.conf_slot, 0);
        pjsua_conf_connect(0, info.conf_slot);
    }

}

Call::Call(QObject *parent)
    : QObject(parent)
{

}

Call::~Call()
{

}

bool Call::makeCall(pjsua_acc_id acc_id, const QString &uri)
{
    pj_str_t str = pj_str("sip:6016@192.168.32.89");
    pjsua_call_setting settings;
    pjsua_call_setting_default(&settings);

    pj_status_t status = pjsua_call_make_call(acc_id, &str, &settings, NULL, NULL, NULL);

    if (status != PJ_SUCCESS) {
        return false;
    }

    return true;
}


} // namespace izdebug
