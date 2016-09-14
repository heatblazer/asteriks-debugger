#ifndef DEFS_H
#define DEFS_H

#define SIP_USER "ilianz"
#define SIP_DOMAIN "example.com"
#define SIP_PASS "secret"

#include <pjmedia.h>

#define IS_DIGIT(D) (((D >= '0') && (D <= '9')))

// if somebody has this werid name ...
#ifdef MATH_PI
#   undef MATH_PI
#   define MATH_PI  (3.14159265)
#else
#   define MATH_PI  (3.14159265)
#endif


#define SLOT_TYPE	    unsigned
#define INVALID_SLOT	    ((SLOT_TYPE)-1)


typedef int (*thCb)(int, void*);
typedef pj_status_t (*pjThrEpoint)(void*);

union port_data {
    pj_int16_t *buff;
};




/** Hijacket structure for the conference bridge
 * This is a port connected to conference bridge.
 */
struct conf_port
{
    pj_str_t		 name;		/**< Port name.			    */
    pjmedia_port	*port;		/**< get_frame() and put_frame()    */
    pjmedia_port_op	 rx_setting;	/**< Can we receive from this port  */
    pjmedia_port_op	 tx_setting;	/**< Can we transmit to this port   */
    unsigned		 listener_cnt;	/**< Number of listeners.	    */
    SLOT_TYPE		*listener_slots;/**< Array of listeners.	    */
    unsigned		 transmitter_cnt;/**<Number of transmitters.	    */

    /* Shortcut for port info. */
    unsigned		 clock_rate;	/**< Port's clock rate.		    */
    unsigned		 samples_per_frame; /**< Port's samples per frame.  */
    unsigned		 channel_count;	/**< Port's channel count.	    */

    /* Calculated signal levels: */
    unsigned		 tx_level;	/**< Last tx level to this port.    */
    unsigned		 rx_level;	/**< Last rx level from this port.  */

    /* The normalized signal level adjustment.
     * A value of 128 (NORMAL_LEVEL) means there's no adjustment.
     */
    unsigned		 tx_adj_level;	/**< Adjustment for TX.		    */
    unsigned		 rx_adj_level;	/**< Adjustment for RX.		    */

    /* Resample, for converting clock rate, if they're different. */
    pjmedia_resample	*rx_resample;
    pjmedia_resample	*tx_resample;

    /* RX buffer is temporary buffer to be used when there is mismatch
     * between port's sample rate or ptime with conference's sample rate
     * or ptime. The buffer is used for sampling rate conversion AND/OR to
     * buffer the samples until there are enough samples to fulfill a
     * complete frame to be processed by the bridge.
     *
     * When both sample rate AND ptime of the port match the conference
     * settings, this buffer will not be created.
     *
     * This buffer contains samples at port's clock rate.
     * The size of this buffer is the sum between port's samples per frame
     * and bridge's samples per frame.
     */
    pj_int16_t		*rx_buf;	/**< The RX buffer.		    */
    unsigned		 rx_buf_cap;	/**< Max size, in samples	    */
    unsigned		 rx_buf_count;	/**< # of samples in the buf.	    */

    /* Mix buf is a temporary buffer used to mix all signal received
     * by this port from all other ports. The mixed signal will be
     * automatically adjusted to the appropriate level whenever
     * there is possibility of clipping.
     *
     * This buffer contains samples at bridge's clock rate.
     * The size of this buffer is equal to samples per frame of the bridge.
     */

    int			 mix_adj;	/**< Adjustment level for mix_buf.  */
    int			 last_mix_adj;	/**< Last adjustment level.	    */
    pj_int32_t		*mix_buf;	/**< Total sum of signal.	    */

    /* Tx buffer is a temporary buffer to be used when there's mismatch
     * between port's clock rate or ptime with conference's sample rate
     * or ptime. This buffer is used as the source of the sampling rate
     * conversion AND/OR to buffer the samples until there are enough
     * samples to fulfill a complete frame to be transmitted to the port.
     *
     * When both sample rate and ptime of the port match the bridge's
     * settings, this buffer will not be created.
     *
     * This buffer contains samples at port's clock rate.
     * The size of this buffer is the sum between port's samples per frame
     * and bridge's samples per frame.
     */
    pj_int16_t		*tx_buf;	/**< Tx buffer.			    */
    unsigned		 tx_buf_cap;	/**< Max size, in samples.	    */
    unsigned		 tx_buf_count;	/**< # of samples in the buffer.    */

    /* When the port is not receiving signal from any other ports (e.g. when
     * no other ports is transmitting to this port), the bridge periodically
     * transmit NULL frame to the port to keep the port "alive" (for example,
     * a stream port needs this heart-beat to periodically transmit silence
     * frame to keep NAT binding alive).
     *
     * This NULL frame should be sent to the port at the port's ptime rate.
     * So if the port's ptime is greater than the bridge's ptime, the bridge
     * needs to delay the NULL frame until it's the right time to do so.
     *
     * This variable keeps track of how many pending NULL samples are being
     * "held" for this port. Once this value reaches samples_per_frame
     * value of the port, a NULL frame is sent. The samples value on this
     * variable is clocked at the port's clock rate.
     */
    unsigned		 tx_heart_beat;

    /* Delay buffer is a special buffer for sound device port (port 0, master
     * port) and other passive ports (sound device port is also passive port).
     *
     * We need the delay buffer because we can not expect the mic and speaker
     * thread to run equally after one another. In most systems, each thread
     * will run multiple times before the other thread gains execution time.
     * For example, in my system, mic thread is called three times, then
     * speaker thread is called three times, and so on. This we call burst.
     *
     * There is also possibility of drift, unbalanced rate between put_frame
     * and get_frame operation, in passive ports. If drift happens, snd_buf
     * needs to be expanded or shrinked.
     *
     * Burst and drift are handled by delay buffer.
     */
    pjmedia_delay_buf	*delay_buf;
};

/// Hijack the pjmedia_conf port to retrieve the data
/// \brief The pjmedia_conf2 struct
///
struct pjmedia_conf2
{
    unsigned		  options;	/**< Bitmask options.		    */
    unsigned		  max_ports;	/**< Maximum ports.		    */
    unsigned		  port_cnt;	/**< Current number of ports.	    */
    unsigned		  connect_cnt;	/**< Total number of connections    */
    pjmedia_snd_port	 *snd_dev_port;	/**< Sound device port.		    */
    pjmedia_port	 *master_port;	/**< Port zero's port.		    */
    char		  master_name_buf[80]; /**< Port0 name buffer.	    */
    pj_mutex_t		 *mutex;	/**< Conference mutex.		    */
    struct conf_port	**ports;	/**< Array of ports.		    */
    unsigned		  clock_rate;	/**< Sampling rate.		    */
    unsigned		  channel_count;/**< Number of channels (1=mono).   */
    unsigned		  samples_per_frame;	/**< Samples per frame.	    */
    unsigned		  bits_per_sample;	/**< Bits per sample.	    */
};


struct dtmf
{
    int		    event;
    pj_uint32_t	    duration;
    int		    ebit_cnt;		    /**< # of E bit transmissions   */
};



// stream hijacking
struct pjmedia_stream2
{
    pjmedia_endpt	    *endpt;	    /**< Media endpoint.	    */
    pjmedia_codec_mgr	    *codec_mgr;	    /**< Codec manager instance.    */
    pjmedia_stream_info	     si;	    /**< Creation parameter.        */
    pjmedia_port	     port;	    /**< Port interface.	    */
    pjmedia_channel	    *enc;	    /**< Encoding channel.	    */
    pjmedia_channel	    *dec;	    /**< Decoding channel.	    */

    pj_pool_t		    *own_pool;	    /**< Only created if not given  */

    pjmedia_dir		     dir;	    /**< Stream direction.	    */
    void		    *user_data;	    /**< User data.		    */
    pj_str_t		     cname;	    /**< SDES CNAME		    */

    pjmedia_transport	    *transport;	    /**< Stream transport.	    */

    pjmedia_codec	    *codec;	    /**< Codec instance being used. */
    pjmedia_codec_param	     codec_param;   /**< Codec param.		    */
    pj_int16_t		    *enc_buf;	    /**< Encoding buffer, when enc's
                         ptime is different than dec.
                         Otherwise it's NULL.	    */

    unsigned		     enc_samples_per_pkt;
    unsigned		     enc_buf_size;  /**< Encoding buffer size, in
                         samples.		    */
    unsigned		     enc_buf_pos;   /**< First position in buf.	    */
    unsigned		     enc_buf_count; /**< Number of samples in the
                         encoding buffer.	    */

    unsigned		     plc_cnt;	    /**< # of consecutive PLC frames*/
    unsigned		     max_plc_cnt;   /**< Max # of PLC frames	    */

    unsigned		     vad_enabled;   /**< VAD enabled in param.	    */
    unsigned		     frame_size;    /**< Size of encoded base frame.*/
    pj_bool_t		     is_streaming;  /**< Currently streaming?. This
                         is used to put RTP marker
                         bit.			    */
    pj_uint32_t		     ts_vad_disabled;/**< TS when VAD was disabled. */
    pj_uint32_t		     tx_duration;   /**< TX duration in timestamp.  */

    pj_mutex_t		    *jb_mutex;
    pjmedia_jbuf	    *jb;	    /**< Jitter buffer.		    */
    char		     jb_last_frm;   /**< Last frame type from jb    */
    unsigned		     jb_last_frm_cnt;/**< Last JB frame type counter*/

    pjmedia_rtcp_session     rtcp;	    /**< RTCP for incoming RTP.	    */

    pj_uint32_t		     rtcp_last_tx;  /**< RTCP tx time in timestamp  */
    pj_uint32_t		     rtcp_interval; /**< Interval, in timestamp.    */
    pj_bool_t		     initial_rr;    /**< Initial RTCP RR sent	    */
    pj_bool_t                rtcp_sdes_bye_disabled;/**< Send RTCP SDES/BYE?*/
    void		    *out_rtcp_pkt;  /**< Outgoing RTCP packet.	    */
    unsigned		     out_rtcp_pkt_size;
                        /**< Outgoing RTCP packet size. */

    /* RFC 2833 DTMF transmission queue: */
    int			     tx_event_pt;   /**< Outgoing pt for dtmf.	    */
    int			     tx_dtmf_count; /**< # of digits in tx dtmf buf.*/
    struct dtmf		     tx_dtmf_buf[32];/**< Outgoing dtmf queue.	    */

    /* Incoming DTMF: */
    int			     rx_event_pt;   /**< Incoming pt for dtmf.	    */
    int			     last_dtmf;	    /**< Current digit, or -1.	    */
    pj_uint32_t		     last_dtmf_dur; /**< Start ts for cur digit.    */
    unsigned		     rx_dtmf_count; /**< # of digits in dtmf rx buf.*/
    char		     rx_dtmf_buf[32];/**< Incoming DTMF buffer.	    */

    /* DTMF callback */
    void		    (*dtmf_cb)(pjmedia_stream*, void*, int);
    void		     *dtmf_cb_user_data;

#if defined(PJMEDIA_HANDLE_G722_MPEG_BUG) && (PJMEDIA_HANDLE_G722_MPEG_BUG!=0)
    /* Enable support to handle codecs with inconsistent clock rate
     * between clock rate in SDP/RTP & the clock rate that is actually used.
     * This happens for example with G.722 and MPEG audio codecs.
     */
    pj_bool_t		     has_g722_mpeg_bug;
                        /**< Flag to specify whether
                         normalization process
                         is needed		    */
    unsigned		     rtp_tx_ts_len_per_pkt;
                        /**< Normalized ts length per packet
                         transmitted according to
                         'erroneous' definition	    */
    unsigned		     rtp_rx_ts_len_per_frame;
                        /**< Normalized ts length per frame
                         received according to
                         'erroneous' definition	    */
    unsigned		     rtp_rx_last_cnt;/**< Nb of frames in last pkt  */
    unsigned		     rtp_rx_check_cnt;
                        /**< Counter of remote timestamp
                         checking */
#endif

#if defined(PJMEDIA_HAS_RTCP_XR) && (PJMEDIA_HAS_RTCP_XR != 0)
    pj_uint32_t		     rtcp_xr_last_tx;  /**< RTCP XR tx time
                                in timestamp.           */
    pj_uint32_t		     rtcp_xr_interval; /**< Interval, in timestamp. */
    pj_sockaddr		     rtcp_xr_dest;     /**< Additional remote RTCP XR
                            dest. If sin_family is
                            zero, it will be ignored*/
    unsigned		     rtcp_xr_dest_len; /**< Length of RTCP XR dest
                                address		    */
#endif

#if defined(PJMEDIA_STREAM_ENABLE_KA) && PJMEDIA_STREAM_ENABLE_KA!=0
    pj_bool_t		     use_ka;	       /**< Stream keep-alive with non-
                            codec-VAD mechanism is
                            enabled?		    */
    pj_timestamp	     last_frm_ts_sent; /**< Timestamp of last sending
                                packet		    */
#endif

#if TRACE_JB
    pj_oshandle_t	    trace_jb_fd;	    /**< Jitter tracing file handle.*/
    char		   *trace_jb_buf;	    /**< Jitter tracing buffer.	    */
#endif

    pj_uint32_t		     rtp_rx_last_ts;        /**< Last received RTP timestamp*/
    pj_status_t		     rtp_rx_last_err;       /**< Last RTP recv() error */
};




#endif // DEFS_H
