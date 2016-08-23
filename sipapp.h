#ifndef SIPAPP_H
#define SIPAPP_H

// qt headers //
#include <QObject>

// pjsua //
#include <pjsua.h>


namespace izdebug {

class Call;

class SipApp : public QObject
{
    Q_OBJECT
public:
    explicit SipApp(QObject* parent=nullptr);
    virtual ~SipApp();
    bool create(const QString& uri);

public slots:

    void makeACall();

private:

    pjsua_acc_id m_acc_id;
    pjsua_config m_config;
    pjsua_transport_config m_transCfg;
    pjsua_acc_config m_accCfg;

    Call*       p_call;
};

}

#endif // SIPAPP_H
