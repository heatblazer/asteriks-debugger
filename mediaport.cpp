#include "mediaport.h"

#include <pjsua.h>

namespace izdebug {

bool MediaPort::create()
{

}

MediaPort::MediaPort(QObject *parent)
    : QObject(parent)
{

}

MediaPort::~MediaPort()
{
    if (p_port != nullptr) {
        pjmedia_port_destroy(p_port);
        p_port = nullptr;
        m_slot = PJSUA_INVALID_ID;
    }
}

} // namespace izdebug

