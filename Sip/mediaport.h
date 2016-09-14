#ifndef MEDIAPORT_H
#define MEDIAPORT_H

// pjmedia //
#include <pjmedia.h>
#include <pjsua.h>

namespace izdebug {

class ConfBridge;

class MediaPort
{
public:
    virtual bool create()=0;
    virtual bool isAdded();
    virtual void setSlot(unsigned slot);
    virtual unsigned getSlot();
    virtual void setSrc(unsigned sink);
    virtual unsigned getSrc();
    virtual pjmedia_port* toPj();
    virtual bool connect(pjsua_conf_port_id src, pjsua_conf_port_id dst);

protected:
    explicit MediaPort();
    virtual ~MediaPort()=0;



protected:
    pjmedia_port* p_port;
    unsigned      m_slot;
    unsigned      m_sink;
    bool m_isOk;
    bool m_isAdded;

    friend class ConfBridge;
};

} // namespace izdebug


#endif // MEDIAPORT_H
