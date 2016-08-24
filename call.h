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
