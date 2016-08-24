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
    explicit MediaPort(QObject* parent=nullptr);
    virtual ~MediaPort();

    virtual bool create();

private:
    pjmedia_port* p_port;
    unsigned      m_slot;

    friend class ConfBridge;
};

} // namespace izdebug


#endif // MEDIAPORT_H
