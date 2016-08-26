#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

namespace izdebug {

class Player : public QObject
{
    Q_OBJECT
    explicit Player(QObject* parent=nullptr);
    virtual ~Player();

    bool create(const char* fname);
    void destroy(void);
    pjmedia_port *pjPort();

public slots:
    void stop();
    void start();

private:
    void _disconnect_and_remove();

private:

    pjmedia_port* p_port;
    unsigned m_slot;
    bool m_isOk;
    bool m_isPlaying;

};

} // namespace izdebug


#endif // PLAYER_H
