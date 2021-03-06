#ifndef SIPAPP_H
#define SIPAPP_H

// qt headers //
#include <QObject>

// pjsua //
#include <pjsua.h>

// defs //
#include "defs.h"

// gui//
#include "Gui/gui.h"

#include "player.h"
#include "recorder.h"
#include "rtsp-rec.h"

namespace izdebug {

class Call;
class Recorder;

class SipApp
{
public:
    static int cb(int argc, void* argv);
    static pj_status_t entry(void* udata);
public:
    static QList<Player*> g_players;
    static Recorder*    g_recorder;
    static RtspRec*    p_rtsp; // access from the cb-s


    static SipApp& Instance();
    static void on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                                  pjsip_rx_data* rx_data);

    static void on_call_state(pjsua_call_id call_id, pjsip_event* ev);

    static void on_call_media_state(pjsua_call_id call_id);

    static void on_stream_created(pjsua_call_id call_id,
                                  pjmedia_stream *strm,
                                  unsigned stream_idx,
                                   pjmedia_port **p_port);

    static void on_stream_destroyed(pjsua_call_id call_id,
                                  pjmedia_stream *strm,
                                  unsigned stream_idx);




    bool create(const QString& uri);

    // both functions get and set current slot,
    // that is gained on call media state
    void setConfSlot(pjsua_conf_port_id conf_slot);
    int getConfSlot(void);
    pjmedia_conf* getConfBridge();

    void makeACall(const char *uri);
    void hupAllCalls(void);

    bool createRtspRecServer(const char* uri, pj_uint16_t port);
    bool createRtspRecClient(const char* uri, pj_uint16_t port);


public:
    explicit SipApp();
    virtual ~SipApp();

    static SipApp* s_instance;
    pjsua_acc_id m_acc_id;
    pjsua_conf_port_id m_current_slot;
    bool m_isCreated;
    char    m_pname[256];
    Recorder*   p_recorder;
    Gui*        p_gui;

    friend class Gui;
    friend class Player;
    friend class Recorder;

};

}

#endif // SIPAPP_H
