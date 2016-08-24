#ifndef SIPAPP_H
#define SIPAPP_H

// qt headers //
#include <QObject>

// pjsua //
#include <pjsua.h>

// defs //
#include "defs.h"

namespace izdebug {

class Call;
class Recorder;

class SipApp : public QObject
{
    Q_OBJECT
public:
    static void on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                                  pjsip_rx_data* rx_data);

    static void on_call_state(pjsua_call_id call_id, pjsip_event* ev);

    static void on_call_media_state(pjsua_call_id call_id);

    static void on_stream_created(pjsua_call_id call_id,
                                  pjmedia_stream *strm,
                                  unsigned stream_idx,
                                   pjmedia_port **p_port);



public:
    explicit SipApp(QObject* parent=nullptr);
    virtual ~SipApp();
    bool create(const QString& uri);

public slots:

    void makeACall(const char *uri);
    void hupCall(void);

private:

    pjsua_acc_id m_acc_id;
    Recorder* p_rec;

};

}

#endif // SIPAPP_H
