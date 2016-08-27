#ifndef PLAYER_H
#define PLAYER_H


// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

// parent //
#include "mediaport.h"

namespace izdebug {

class Player : public MediaPort
{
    Q_OBJECT
    explicit Player(const QString& fname, QObject* parent=nullptr);
    virtual ~Player();

    bool create();
    void destroy(void);
    pjmedia_port *pjPort();

public slots:
    void stop();
    void start();

private:
    void _disconnect_and_remove();
    bool _create(const char* fname);

private:

    pjmedia_port* p_port;
    QString m_fname;
    unsigned m_slot;
    bool m_isOk;
    bool m_isPlaying;


};

} // namespace izdebug


#endif // PLAYER_H
