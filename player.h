#ifndef PLAYER_H
#define PLAYER_H

#include "mediaport.h"

#include <QTimer>

namespace izdebug {

class Player : public MediaPort
{
public:
    explicit Player(const QString& fname);
    virtual ~Player();
    bool create();
    void doWork(void* data);
    void play();
    void stop();
    void playToConf();

private:
    void _disconnect_and_remove();
private:
    pjmedia_snd_port* p_sndPort;
    QString m_fname;
    bool    m_isPlaying;

};

} // namespace izdebug
#endif // PLAYER_H
