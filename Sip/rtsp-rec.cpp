#include "rtsp-rec.h"

#include "mem-pool.h" // memory allocs

#include <pjsua-lib/pjsua_internal.h> // pjsua_var


namespace izdebug {

RtspRec::RtspRec(const char* host, pj_uint16_t port, bool is_server)
    : m_port(port),
      m_master(nullptr),
      m_codec_info(nullptr),
      m_isStreaming(false),
      m_isRecording(false),
      m_isServer(is_server)
{
    m_url = pj_str((char*)host);

}

RtspRec::~RtspRec()
{
    pjmedia_transport* tp = pjmedia_stream_get_transport(p_stream);
    pjmedia_stream_destroy(p_stream);
    pjmedia_transport_close(tp);
}

bool RtspRec::create()
{
    // other stuff if needed
    bool res = false;
    res = _init_codecs();
    if (res) {
        res = _find_codecs(NULL);
        if (res) {
            res = _create_stream();
        }
    }

    return res;
}

void RtspRec::setSlot(unsigned slot)
{
    MediaPort::setSlot(slot);
}

unsigned RtspRec::getSlot()
{
    return MediaPort::getSlot();
}

void RtspRec::setSrc(unsigned sink)
{
    MediaPort::setSrc(sink);
}

unsigned RtspRec::getSrc()
{
    return MediaPort::getSrc();
}

pjmedia_port *RtspRec::toPj()
{
    return MediaPort::toPj();
}

bool RtspRec::connect(pjsua_conf_port_id src, pjsua_conf_port_id dst)
{
    return MediaPort::connect(src, dst);
}

bool RtspRec::start_streaming()
{
    if (asServer()) {
        if (!m_isStreaming) {
            pjmedia_conf_connect_port(pjsua_var.mconf, getSrc(), getSlot(), 0);
            m_isStreaming = true;
        }

        return m_isStreaming;
    }
    return false;
}

bool RtspRec::start_recording()
{
    if (!asServer()) {
        if(!m_isStreaming) {
            pjmedia_conf_connect_port(pjsua_var.mconf, getSrc(), getSlot(), 0);
            m_isStreaming = true;
        }
        return m_isStreaming;
    }
    return false;
}

bool RtspRec::isStreaming()
{
    return m_isStreaming;
}

bool RtspRec::asServer()
{
    return m_isServer;
}

/// first register audioc codecs
/// \brief RtspRec::_init_codecs
/// \return
///
bool RtspRec::_init_codecs()
{
    bool res = false;
    pj_status_t s = pjmedia_codec_register_audio_codecs(pjsua_var.med_endpt, NULL);

    if (s == PJ_SUCCESS) {
        res = true;
    }
    return res;
}

bool RtspRec::_create_stream()
{
    if (!m_isOk) {

        const pjmedia_codec_info* codec_info = NULL;
        pjmedia_codec_mgr_get_codec_info(pjmedia_endpt_get_codec_mgr(
                                             pjsua_var.med_endpt),
                                             0,
                                             &codec_info);


        pjmedia_stream_info info;
        pjmedia_transport *transport = NULL;
        pj_status_t status;


        /* Reset stream info. */
        pj_bzero(&info, sizeof(info));


        /* Initialize stream info formats */
        info.type = PJMEDIA_TYPE_AUDIO;
        info.dir = PJMEDIA_DIR_ENCODING_DECODING; // send and rcv

        pj_memcpy(&info.fmt, codec_info, sizeof(pjmedia_codec_info));
        info.tx_pt = codec_info->pt;
        info.rx_pt = codec_info->pt;
        info.ssrc = pj_rand();


        /* Copy remote address */
        pj_memcpy(&info.rem_addr, &m_socket, sizeof(pj_sockaddr_in));

        /* If remote address is not set, set to an arbitrary address
         * (otherwise stream will assert).
         */
        if (info.rem_addr.addr.sa_family == 0) {
            const pj_str_t addr = pj_str("127.0.0.1");
            pj_sockaddr_in_init(&info.rem_addr.ipv4, &addr, 0);
        }


        /* Create media transport */
        status = pjmedia_transport_udp_create(pjsua_var.med_endpt,
                                              NULL,
                                              m_port,
                                              0,
                                              &transport);
        if (status != PJ_SUCCESS) {
            return m_isOk;
        }


        /* Now that the stream info is initialized, we can create the
         * stream.
         */

        status = pjmedia_stream_create(
                        pjsua_var.med_endpt,
                        Pool::Instance().toPjPool(),
                        &info,
                        transport,
                        NULL, &p_stream);

        if (status != PJ_SUCCESS) {
            pjmedia_transport_close(transport);
            return m_isOk;
        }

        // create the stream port
        status = pjmedia_stream_get_port(p_stream, &p_port);
        if (status != PJ_SUCCESS) {
            return m_isOk;
        }
        pj_str_t name = pj_str("RTSP stream port");

        // add conf port to the conference bridge
        pjmedia_conf_add_port(pjsua_var.mconf,
                              Pool::Instance().toPjPool(),
                              p_port,
                              &name,
                              &m_slot
                              );
        // finally
        m_isOk = true;

    }
    return m_isOk;
}

bool RtspRec::_find_codecs(const char* id)
{
    bool res = false;
    pjmedia_codec_mgr* codec_mgr  = NULL;
    if (id) {
        unsigned count = 1;
        pj_str_t codec = pj_str((char*)id);
        codec_mgr = pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt);
        if (codec_mgr == NULL) {
            return res;
        }


        pj_status_t status = pjmedia_codec_mgr_find_codecs_by_id(codec_mgr,
                                                                 &codec,
                                                                 &count,
                                                                 &m_codec_info,
                                                                 NULL);
        if(status != PJ_SUCCESS) {
            return res;
        }
        res = true;
    } else {
    // default to PCMU
        pjmedia_codec_mgr_get_codec_info(pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt),
                                         0,
                                         &m_codec_info);

        res = true;

    }

    return res;
}


#if 0
PJ_ASSERT_RETURN(status == PJ_SUCCESS, 1);


if (play_file) {
unsigned wav_ptime;

wav_ptime = PJMEDIA_PIA_PTIME(&stream_port->info);
status = pjmedia_wav_player_port_create(pool, play_file, wav_ptime,
                    0, -1, &play_file_port);
if (status != PJ_SUCCESS) {
    app_perror(THIS_FILE, "Unable to use file", status);
    goto on_exit;
}

status = pjmedia_master_port_create(pool, play_file_port, stream_port,
                    0, &master_port);
if (status != PJ_SUCCESS) {
    app_perror(THIS_FILE, "Unable to create master port", status);
    goto on_exit;
}

status = pjmedia_master_port_start(master_port);
if (status != PJ_SUCCESS) {
    app_perror(THIS_FILE, "Error starting master port", status);
    goto on_exit;
}

printf("Playing from WAV file %s..\n", play_file);
#endif

} // namespace izdebug
