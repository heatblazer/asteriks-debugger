#ifndef PLAYER_H
#define PLAYER_H

#include "mediaport.h"

namespace izdebug {

class Player : public MediaPort
{
public:
    explicit Player(const char *fname);
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
    char    m_fname[256];
    bool    m_isPlaying;

};

} // namespace izdebug
#endif // PLAYER_H
