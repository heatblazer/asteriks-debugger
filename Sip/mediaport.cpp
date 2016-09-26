#include "mediaport.h"

#include <pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

namespace izdebug {


MediaPort::MediaPort()
    : p_port(nullptr),
      m_slot(0),
      m_sink(0),
      m_isOk(false)
{

}


bool MediaPort::isAdded()
{
    return m_isAdded;

}

void MediaPort::setSlot(unsigned slot)
{
    m_slot = slot;
}

unsigned MediaPort::getSlot()
{
    return m_slot;

}

unsigned MediaPort::getSrc()
{
    return m_sink;
}

pjmedia_port *MediaPort::toPj()
{
    return p_port;
}

bool MediaPort::connect(pjsua_conf_port_id src, pjsua_conf_port_id dst)
{
    //pj_status_t s = pjmedia_conf_connect_port(pjsua_var.mconf, src, dst, 0);
    //if (s != PJ_SUCCESS) {
    //    return false;
    //}
    (void) src;
    (void) dst;
    return true;
}

void MediaPort::setSrc(unsigned sink)
{
    m_sink = sink;
}

// do the work of destruction here remove destroyings from derivates
MediaPort::~MediaPort()
{
    if (p_port != nullptr) {
        pjmedia_port_destroy(p_port);
        p_port = nullptr;
        m_slot = PJSUA_INVALID_ID;
    }
}

} // namespace izdebug

