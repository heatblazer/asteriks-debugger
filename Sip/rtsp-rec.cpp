#include "rtsp-rec.h"

#include "mem-pool.h" // memory allocs

#include <pjsua-lib/pjsua_internal.h> // pjsua_var


namespace izdebug {

pjmedia_port* RtspRec::g_Port = nullptr;

RtspRec::RtspRec(const char* host, pj_uint16_t port, bool is_server)
    : MediaPort(),
      m_player(nullptr),
      m_rec(nullptr),
      m_master(nullptr),
      m_port(port),
      p_stream(nullptr),
      p_snd_port(nullptr),
      m_codec_info(nullptr),
      m_isStreaming(false),
      m_isRecording(false),
      m_isServer(is_server)
{
    m_url = pj_str((char*)host);

}

RtspRec::~RtspRec()
{
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }

    if (m_rec) {
        delete m_rec;
        m_rec = nullptr;
    }

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
            g_Port = p_port;
            res =  _create_recorder("test_rtsp_rec.wav") &&
                    _create_player("assets/test.wav");
            res = _create_sound();
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

        pjmedia_codec_mgr_get_codec_info(pjmedia_endpt_get_codec_mgr(
                                             pjsua_var.med_endpt),
                                             0,
                                             &m_codec_info);


        pjmedia_stream_info info;
        pjmedia_transport *transport = NULL;
        pj_status_t status;


        /* Reset stream info. */
        pj_bzero(&info, sizeof(info));


        /* Initialize stream info formats */
        info.type = PJMEDIA_TYPE_AUDIO; // audio only

        if(m_isServer) {
            info.dir = PJMEDIA_DIR_ENCODING; // send as server
        } else {
            info.dir = PJMEDIA_DIR_DECODING; // recv as client
        }

        pj_memcpy(&info.fmt, m_codec_info, sizeof(pjmedia_codec_info));
        info.tx_pt = m_codec_info->pt;
        info.rx_pt = m_codec_info->pt;
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


        status = pjmedia_codec_mgr_get_default_param(
                        pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt),
                        m_codec_info,
                        &codec_param);
        /* Should be ok, as create_stream() above succeeded */
        pj_assert(status == PJ_SUCCESS);

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

bool RtspRec::_create_recorder(const char *fname)
{
    bool res = false;
    m_rec = new RtspRec::Recorder(fname);
    res = m_rec->create();
    return res;
}

bool RtspRec::_create_player(const char *fname)
{
    bool res = false;
    m_player = new RtspRec::Player(fname);
    res = m_player->create();
    return res;
}

bool RtspRec::_create_sound()
{
    bool res = true;
    pj_status_t status;
    if (m_isServer) {
        status = pjmedia_snd_port_create_player(Pool::Instance().toPjPool(),
                                                -1,
                                                PJMEDIA_PIA_SRATE(&p_port->info),
                                                PJMEDIA_PIA_CCNT(&p_port->info),
                                                PJMEDIA_PIA_SPF(&p_port->info),
                                                PJMEDIA_PIA_BITS(&p_port->info),
                                                0, &p_snd_port);
        if (status != PJ_SUCCESS) {
            res = false;
        }
        status = pjmedia_snd_port_connect(p_snd_port, p_port);
        if (status != PJ_SUCCESS) {
            res = false;
        }
        pjmedia_stream_start(p_stream);
    } else {
        status = pjmedia_snd_port_create_rec(Pool::Instance().toPjPool(),
                                                -1,
                                                PJMEDIA_PIA_SRATE(&p_port->info),
                                                PJMEDIA_PIA_CCNT(&p_port->info),
                                                PJMEDIA_PIA_SPF(&p_port->info),
                                                PJMEDIA_PIA_BITS(&p_port->info),
                                                0, &p_snd_port);
        if (status != PJ_SUCCESS) {
            res = false;
        }
        status = pjmedia_snd_port_connect(p_snd_port, p_port);
        if (status != PJ_SUCCESS) {
            res = false;
        }
    }

    return res;
}


RtspRec::Recorder::Recorder(const char *fname)
    : MediaPort()
{
    strcpy(m_filename, fname);
}

RtspRec::Recorder::~Recorder()
{
}

bool RtspRec::Recorder::create()
{
    bool res = true;
    pj_status_t status;
    status = pjmedia_wav_writer_port_create(Pool::Instance().toPjPool(),
                                            m_filename,
                                            PJMEDIA_PIA_SRATE(&g_Port->info),
                                            PJMEDIA_PIA_CCNT(&g_Port->info),
                                            PJMEDIA_PIA_SPF(&g_Port->info),
                                            PJMEDIA_PIA_BITS(&g_Port->info),
                                            0,
                                            0,
                                            &this->p_port);

    if (status != PJ_SUCCESS) {
        res = false;
    }

    return res;

}


RtspRec::Player::Player(const char *fname)
    : MediaPort()
{
    strcpy(m_filename, fname);
}

RtspRec::Player::~Player()
{
}

bool RtspRec::Player::create()
{
    bool res = true;
    pj_status_t status;
    unsigned wav_ptime = PJMEDIA_PIA_PTIME(&g_Port->info);
    status = pjmedia_wav_player_port_create(Pool::Instance().toPjPool(),
                                            m_filename,
                                            wav_ptime,
                                            0, 0, &this->p_port);
    if (status != PJ_SUCCESS) {
        res = false;
    }

    return res;
}

} // namespace izdebug
