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
public:
    virtual bool create()=0;
    virtual bool isAdded();
    virtual void setSlot(unsigned slot);
    virtual unsigned getSlot();
    virtual void setSink(unsigned sink);
    virtual unsigned getSink();
    virtual pjmedia_port* toPj();

protected:
    explicit MediaPort(QObject* parent=nullptr);
    virtual ~MediaPort()=0;



protected:
    pjmedia_port* p_port;
    unsigned      m_slot;
    unsigned      m_sink;
    bool m_isOk;
    bool m_isAdded;

    friend class ConfBridge;
};

} // namespace izdebug


#endif // MEDIAPORT_H
