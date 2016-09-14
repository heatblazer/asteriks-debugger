#ifndef CONFBRIDGE_H
#define CONFBRIDGE_H

// qt //
#include <QObject>

// pjlib //
#include <pjmedia.h>

namespace izdebug {

class MediaPort;

class ConfBridge : public QObject
{
    Q_OBJECT
public:
    explicit ConfBridge(QObject* parent);
    virtual ~ConfBridge();

    bool create(void);

signals:
    void portAdded(MediaPort* port); // no constnes, since we may want to do sth.

public slots:

private:

    pjmedia_snd_port* p_sndDev;
    pjmedia_conf*     p_bridge;

    QList<MediaPort*> m_ports;
};

} // namespace izdebug


#endif // CONFBRIDGE_H
