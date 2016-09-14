#include "call.h"

namespace izdebug {


Call::Call(QObject *parent)
    : QObject(parent)
{

}

Call::~Call()
{

}

bool Call::makeCall(pjsua_acc_id acc_id, const QString &uri)
{
    pj_str_t str = pj_str(uri.toLatin1().data());

    pj_status_t status = pjsua_call_make_call(acc_id, &str, 0, NULL, NULL, NULL);

    if (status != PJ_SUCCESS) {
        return false;
    }

    return true;
}


} // namespace izdebug
