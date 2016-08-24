#include "confbridge.h"

// pjsip //
#include <pjsua.h>
#include <pjsua-lib/pjsua_internal.h> // pjsua_var

// pool //
#include "mem-pool.h"

// port //
#include "mediaport.h"

namespace izdebug {

ConfBridge::ConfBridge(QObject *parent)
    : p_sndDev(nullptr),
      p_bridge(nullptr)
{
    for(int i=0; i < m_ports.count(); ++i) {
        m_ports[i]->p_port = nullptr;
        m_ports[i]->m_slot = PJSUA_INVALID_ID;
    }

}

ConfBridge::~ConfBridge()
{

}

bool ConfBridge::create()
{
    // first disconnect the default

    pj_status_t status;
    // disable the defaults
    pjmedia_port* old_port = pjsua_set_no_snd_dev();


    // todo !

}

} // namespace ozdebug
