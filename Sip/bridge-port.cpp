#include "bridge-port.h"

#include "mem-pool.h"

#include <pjsua-lib/pjsua_internal.h> // pjsua var

namespace  izdebug {

BridgePort::BridgePort(pjmedia_port *put, pjmedia_port *get)
    : put_port(put),
      get_port(get)
{

}

BridgePort::~BridgePort()
{

}

bool BridgePort::create()
{
    bool res = _create_bidirectional();
    return res;
}

bool BridgePort::_create_bidirectional()
{
    if (!m_isOk) {
        pj_status_t status;
        status = pjmedia_bidirectional_port_create(Pool::Instance().toPjPool(),
                                                   get_port,
                                                   put_port,
                                                   &p_port);

        if (status != PJ_SUCCESS) {
            return m_isOk;
        }
        pj_str_t name = pj_str("Bidirectional port");
        pjmedia_conf_add_port(pjsua_var.mconf, Pool::Instance().toPjPool(),
                              p_port,
                              &name,
                              &m_slot);

    }
}

} // izdebug


