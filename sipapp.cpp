#include "sipapp.h"

// pjsua //
#include <pjsua-lib/pjsua_internal.h>
#include "mem-pool.h"
// C++ //
#include <iostream>

// recorder //
#include "recorder.h"

// console //
#include "gui.h"

// thread //
#include "thread.h"

static inline void lockStack(void)
{
    for(;;) {
        // break here
        int i = 0xDEADCAFE;
        (void)i;
    }
}


namespace izdebug {


void SipApp::on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rx_data)
{
    (void) rx_data;
    pjsua_call_info info;
    // fill info struct
    (void) acc_id;
    pjsua_call_get_info(call_id, &info);

    pjsua_call_answer(call_id, 200, NULL, NULL);
}

void SipApp::on_call_state(pjsua_call_id call_id, pjsip_event *ev)
{
    pjsua_call_info info;
    (void) ev;
    pjsua_call_get_info(call_id, &info);

}

void SipApp::on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info info;
    pjsua_call_get_info(call_id, &info);

    if (info.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        pjsua_conf_connect(info.conf_slot, 0);
        pjsua_conf_connect(0, info.conf_slot);
    }

}

#if 0
typedef struct pjmedia_stream_info
{
    pjmedia_type	type;	    /**< Media type (audio, video)	    */
    pjmedia_tp_proto	proto;	    /**< Transport protocol (RTP/AVP, etc.) */
    pjmedia_dir		dir;	    /**< Media direction.		    */
    pj_sockaddr		rem_addr;   /**< Remote RTP address		    */
    pj_sockaddr		rem_rtcp;   /**< Optional remote RTCP address. If
                     sin_family is zero, the RTP address
                     will be calculated from RTP.	    */
#if defined(PJMEDIA_HAS_RTCP_XR) && (PJMEDIA_HAS_RTCP_XR != 0)
    pj_bool_t		rtcp_xr_enabled;
                    /**< Specify whether RTCP XR is enabled.*/
    pj_uint32_t		rtcp_xr_interval; /**< RTCP XR interval.            */
    pj_sockaddr		rtcp_xr_dest;/**<Additional remote RTCP XR address.
                         This is useful for third-party (e.g:
                     network monitor) to monitor the
                     stream. If sin_family is zero,
                     this will be ignored.		    */
#endif
    pjmedia_codec_info	fmt;	    /**< Incoming codec format info.	    */
    pjmedia_codec_param *param;	    /**< Optional codec param.		    */
    unsigned		tx_pt;	    /**< Outgoing codec paylaod type.	    */
    unsigned		rx_pt;	    /**< Incoming codec paylaod type.	    */
    unsigned		tx_maxptime;/**< Outgoing codec max ptime.	    */
    int		        tx_event_pt;/**< Outgoing pt for telephone-events.  */
    int			rx_event_pt;/**< Incoming pt for telephone-events.  */
    pj_uint32_t		ssrc;	    /**< RTP SSRC.			    */
    pj_uint32_t		rtp_ts;	    /**< Initial RTP timestamp.		    */
    pj_uint16_t		rtp_seq;    /**< Initial RTP sequence number.	    */
    pj_uint8_t		rtp_seq_ts_set;
                    /**< Bitmask flags if initial RTP sequence
                         and/or timestamp for sender are set.
                     bit 0/LSB : sequence flag
                     bit 1     : timestamp flag 	    */
    int			jb_init;    /**< Jitter buffer init delay in msec.
                     (-1 for default).		    */
    int			jb_min_pre; /**< Jitter buffer minimum prefetch
                     delay in msec (-1 for default).    */
    int			jb_max_pre; /**< Jitter buffer maximum prefetch
                     delay in msec (-1 for default).    */
    int			jb_max;	    /**< Jitter buffer max delay in msec.   */

#if defined(PJMEDIA_STREAM_ENABLE_KA) && PJMEDIA_STREAM_ENABLE_KA!=0
    pj_bool_t		use_ka;	    /**< Stream keep-alive and NAT hole punch
                     (see #PJMEDIA_STREAM_ENABLE_KA)
                     is enabled?			    */
#endif
    pj_bool_t           rtcp_sdes_bye_disabled;
                                    /**< Disable automatic sending of RTCP
                                         SDES and BYE.                      */
} pjmedia_stream_info;

typedef struct pjmedia_codec_info
{
    pjmedia_type    type;	    /**< Media type.			*/
    unsigned	    pt;		    /**< Payload type (can be dynamic). */
    pj_str_t	    encoding_name;  /**< Encoding name.			*/
    unsigned	    clock_rate;	    /**< Sampling rate.			*/
    unsigned	    channel_cnt;    /**< Channel count.			*/
} pjmedia_codec_info;



#endif

void SipApp::on_stream_created(pjsua_call_id call_id, pjmedia_stream *strm,
                               unsigned stream_idx, pjmedia_port **p_port)
{
    (void)p_port;

    std::cout << "Call id: " << call_id << std::endl;
    pjsua_stream_info info;
    pjsua_call_get_stream_info(call_id, stream_idx, &info);

    pjmedia_transport_info tinfo;
    pjsua_call_get_med_transport_info(call_id, stream_idx, &tinfo);
    pjmedia_stream_info sinfo;
    pjmedia_stream_get_info(strm, &sinfo);


    // get the conf port id
    pjsua_conf_port_id conf_id = pjsua_call_get_conf_port(call_id);

    pjsua_conf_port_info conf_info;
    pjsua_conf_get_port_info(conf_id, &conf_info);


    static char con[1024]={0};
    char name[64] = {0};
    memcpy(name, sinfo.fmt.encoding_name.ptr, sinfo.fmt.encoding_name.slen);
    sprintf(con, "type:[%d]\npayload:[%d]\nenc_name[%s]\n"
            "clock rate:[%d]\nchan_cnt[%d]\n"
            "rxlevel[%f]\ttxlevel[%f]\n"
            "samples:[%d]\tbits[%d]\n",
            sinfo.fmt.type, sinfo.fmt.pt, name,
            sinfo.fmt.clock_rate, sinfo.fmt.channel_cnt,
            conf_info.rx_level_adj, conf_info.tx_level_adj,
            conf_info.samples_per_frame, conf_info.bits_per_sample);

    Console::Instance().putData(QByteArray(con));

    // play PCMU to the port

#if 0
    pjsua_player_id player_id;
    static pj_str_t str = pj_str("test_pcma16.wav");
    pjsua_player_create(&str, 0, &player_id);
    pjsua_player_get_port(player_id, &player_port);
    pjsua_conf_connect(0, pjsua_player_get_conf_port(player_id));
#endif

}



SipApp::SipApp(QObject *parent)
    : QObject(parent)
{

}

SipApp::~SipApp()
{
    pjsua_destroy();
}

bool SipApp::create(const QString &uri)
{
    pj_status_t status;
    status = pjsua_create();
    if (status != PJ_SUCCESS) {
        return false;
    }

    status = pjsua_verify_url(uri.toLatin1().constData());
    if (status != PJ_SUCCESS) {
        return false;
    }

    {
        pjsua_config cfg;
        pjsua_config_default(&cfg);

        cfg.cb.on_incoming_call = &SipApp::on_incomming_call;
        cfg.cb.on_call_media_state = &SipApp::on_call_media_state;
        cfg.cb.on_call_state = &SipApp::on_call_state;
        cfg.cb.on_stream_created = &SipApp::on_stream_created;

        status = pjsua_init(&cfg, NULL, NULL);

        if (status != PJ_SUCCESS) {
            return false;
        }
    }

    {
        pjsua_transport_config cfg;
        pjsua_transport_id id;
        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;
        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &id);
        if (status != PJ_SUCCESS) {
            return false;
        }

    }

    // start pjsua
    status = pjsua_start();

    if (status != PJ_SUCCESS) {
        return false;
    }

    {
        pjsua_acc_config cfg;
        pjsua_acc_config_default(&cfg);

        char idstr[128]={0};
        sprintf(idstr, "sip:%s@%s", SIP_USER, SIP_DOMAIN);
        cfg.id = pj_str(idstr);
        cfg.reg_uri = pj_str("sip:example.com");
        cfg.cred_count = 1;
        cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
        cfg.cred_info[0].scheme = pj_str("digest");
        cfg.cred_info[0].username = pj_str(SIP_USER);
        cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
        cfg.cred_info[0].data = pj_str(SIP_PASS);

        status = pjsua_acc_add(&cfg, PJ_TRUE, &m_acc_id);

        if (status != PJ_SUCCESS) {
            return false;
        }

    }

    return true;
}

void SipApp::makeACall(const char* uri)
{
    char sipuri[128]={0};
    strcpy(sipuri, uri);
    pj_str_t s = pj_str(sipuri);
    pj_status_t status = pjsua_call_make_call(m_acc_id, &s
                                  , 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) {
        std::cout << "Failed to make a call!" << std::endl;
    }

}

void SipApp::hupCall()
{
    pjsua_call_hangup_all();
    Gui::g_recorder.stop();
}

} // namespace izdebug
