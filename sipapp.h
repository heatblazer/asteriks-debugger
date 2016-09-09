#ifndef SIPAPP_H
#define SIPAPP_H

// qt headers //
#include <QObject>

// pjsua //
#include <pjsua.h>

// defs //
#include "defs.h"

// gui//
#include "gui.h"

#include "player.h"
#include "recorder.h"

namespace izdebug {

class Call;
class Recorder;

class SipApp : public QObject
{
    Q_OBJECT
public:
    static SipApp& Instance();
    static void on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                                  pjsip_rx_data* rx_data);

    static void on_call_state(pjsua_call_id call_id, pjsip_event* ev);

    static void on_call_media_state(pjsua_call_id call_id);

    static void on_stream_created(pjsua_call_id call_id,
                                  pjmedia_stream *strm,
                                  unsigned stream_idx,
                                   pjmedia_port **p_port);



    bool create(const QString& uri);

    // both functions get and set current slot,
    // that is gained on call media state
    void setConfSlot(pjsua_conf_port_id conf_slot);
    int getConfSlot(void);
signals:
    void startPlayer(Player*  plr);
    void startPlayerTimer();
    void startRecorderTimer();
public slots:

    void makeACall(const char *uri);
    void hupCall(void);
    void stopWav();
    void playLoadedFile(const char* fname);
    void hPlayTimer();
    void hRecorderTimer();

private:
public:
    explicit SipApp(QObject* parent=nullptr);
    virtual ~SipApp();

    static SipApp* s_instance;
    pjsua_acc_id m_acc_id;
    pjsua_conf_port_id m_current_slot;
    char    m_pname[256];
    Player*     p_player;
    Recorder*   p_recorder;
    Gui*        p_gui;

    friend class Gui;

};

}

#endif // SIPAPP_H
