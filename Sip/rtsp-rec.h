#ifndef RTSPREC_H
#define RTSPREC_H

#include "mediaport.h" // base
#include "bridge-port.h" // connect ports

namespace izdebug {

class RtspRec : public MediaPort
{
public:
    RtspRec(const char* host, pj_uint16_t port);
    ~RtspRec();
    bool create();
    void setSlot(unsigned slot);
    unsigned getSlot();
    void setSrc(unsigned sink);
    unsigned getSrc();
    pjmedia_port *toPj();
    bool connect(pjsua_conf_port_id src, pjsua_conf_port_id dst);
    bool start_streaming();
    bool isStreaming();

private:
    bool _create_stream();
private:
    pj_sockaddr_in  m_socket;
    pj_str_t        m_url;
    pj_uint16_t     m_port;
    pjmedia_stream* p_stream;
    BridgePort*     p_bridge;
    bool            m_isStreaming;
};

} // namespace izdebug


#endif // RTSPREC_H
