#include "mediaport.h"

#include <pjsua.h>

namespace izdebug {


MediaPort::MediaPort(QObject *parent)
    : QObject(parent),
      p_port(nullptr),
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

unsigned MediaPort::getSink()
{
    return m_sink;
}

pjmedia_port *MediaPort::toPj()
{
    return p_port;
}

void MediaPort::setSink(unsigned sink)
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

