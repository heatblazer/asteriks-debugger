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

class SipApp : public QObject
{
    Q_OBJECT
public:
    explicit SipApp(QObject* parent=nullptr);
    virtual ~SipApp();
    bool create(const QString& uri);

public slots:

    void makeACall(const QString& uri);

private:

    pjsua_acc_id m_acc_id;
    Call*       p_call;
};

}

#endif // SIPAPP_H
