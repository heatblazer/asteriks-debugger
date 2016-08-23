#ifndef CALL_H
#define CALL_H

// qt headder //
#include <QObject>

// pjsua //
#include <pjsua.h>

namespace izdebug {

class Call : public QObject
{
    Q_OBJECT

public:

    static void on_incomming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
                                  pjsip_rx_data* rx_data);

    static void on_call_state(pjsua_call_id call_id, pjsip_event* ev);

    static void on_call_media_state(pjsua_call_id call_id);


    explicit Call(QObject* parent=nullptr);
    virtual ~Call();

    bool makeCall(pjsua_acc_id acc_id, const QString& uri);

signals:
    void hasMessage(const QString& msg);


private:

     QString         m_message;
};

} // namespace izdebug

#endif // CALL_H
