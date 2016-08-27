#ifndef MEDIAPORT_H
#define MEDIAPORT_H

// qt //
#include <QObject>

// pjmedia //
#include <pjmedia.h>


namespace izdebug {

class ConfBridge;

class MediaPort : public QObject
{
protected:
    explicit MediaPort(QObject* parent=nullptr);
    virtual ~MediaPort()=0;

    virtual bool create()=0;

private:
    pjmedia_port* p_port;
    unsigned      m_slot;

    friend class ConfBridge;
};

} // namespace izdebug


#endif // MEDIAPORT_H
