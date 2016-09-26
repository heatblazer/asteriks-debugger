#ifndef RTSPREC_H
#define RTSPREC_H

#include "mediaport.h" // base
#include "bridge-port.h" // connect ports

namespace izdebug {

/// this class will act as CLIENT/SERVER
/// to test it must staart 2 instances of the program
/// then to clicke RTSP (CLIENT) on the one instance
/// and RTSP (SERVER) on the second so the server will
/// start transmit realtime streaming to the client
/// client will implement
/// 1. setup
/// 2. play
/// 3. options
/// 4. teardown
/// Server will have
/// 1. RTSP/1.0 200 OK responses
/// \brief The RtspRec class
///
class RtspRec : public MediaPort
{
public:
    RtspRec(const char* host, pj_uint16_t port, bool is_server);
    ~RtspRec();
    bool create();
    void setSlot(unsigned slot);
    unsigned getSlot();
    void setSrc(unsigned sink);
    unsigned getSrc();
    pjmedia_port *toPj();
    bool connect(pjsua_conf_port_id src, pjsua_conf_port_id dst);
    bool start_streaming();
    bool start_recording();
    bool isStreaming();

    // rtsp specific as CLIENT
    void options(const char* str);
    void describe(const char* str);
    void setup(const char* str);
    void play(const char* str);
    void teardown(const char* str);

    bool asServer();

    // rtsp specific as SERVE


private:
    bool _init_codecs();
    bool _create_stream();
    bool _find_codecs(const char *id);

private:
    // clock needed to the conf to stream
    pjmedia_master_port* m_master; // provide ticks for get/put frame cb()
    const pjmedia_codec_info *m_codec_info;
    pj_sockaddr_in  m_socket;
    pj_str_t        m_url;
    pj_uint16_t     m_port;
    pjmedia_stream* p_stream;
    BridgePort*     p_bridge;
    bool            m_isStreaming;
    bool            m_isRecording;
    bool            m_isServer;
};

} // namespace izdebug


#endif // RTSPREC_H
