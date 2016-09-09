#include "defs.h"
#include "player.h"
#include "mem-pool.h"

#include <iostream>

#include <pjsua.h>
#include <pjsua-lib/pjsua_internal.h>

namespace izdebug {

Player::Player(const QString &fname, QObject *parent)
    : MediaPort(parent),
      p_sndPort(nullptr),
      m_isPlaying(false)
{
    m_fname=fname;    
}

Player::~Player()
{
    if (m_isOk) {
        _disconnect_and_remove();
    }
}

bool Player::create()
{
    if (!m_isOk) {
        pjmedia_port* conf = pjmedia_conf_get_master_port(pjsua_var.mconf);

        if (conf == NULL) {
            // no conf bridge? why?
        }

        pjmedia_wav_player_port_create(Pool::Instance().toPjPool(),
                                       m_fname.toLatin1().constData(),
                                       PJMEDIA_PIA_CCNT(&conf->info),
                                       0, 0,
                                       &p_port
                                       );

        pjmedia_snd_port_create_player(Pool::Instance().toPjPool(),
                                    -1,
                                    PJMEDIA_PIA_SRATE(&conf->info),
                                    PJMEDIA_PIA_CCNT(&conf->info),
                                    PJMEDIA_PIA_SPF(&conf->info),
                                    PJMEDIA_PIA_BITS(&conf->info),
                                    0,
                                    &p_sndPort);

         {

            pjmedia_snd_port_connect(p_sndPort, p_port);
            pj_status_t status = pjmedia_conf_add_port(pjsua_var.mconf,
                                       Pool::Instance().toPjPool(),
                                       p_port,
                                       NULL,
                                       &m_slot);

            if (status != PJ_SUCCESS) {
                pjmedia_port_destroy(p_port);
                m_isAdded = true;
                return m_isOk;
            }
        }
        m_isOk = true;


    }

    return m_isOk;
}

void Player::play()
{
    if(!m_isPlaying) {
        m_isPlaying = true;
    }
    emit update(m_isPlaying);
}

void Player::stop()
{

    if(m_isPlaying) {
        m_isPlaying = false;
    }
    emit update(m_isPlaying);
}


void Player::test()
{
    std::cout << "PLAYER TIMER!" << std::endl;
}

void Player::_disconnect_and_remove()
{
    if (m_isOk) {
        pj_status_t status = pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     0, m_slot);

        if (status == PJ_SUCCESS) {
            m_isOk = false; // we can create it again
            m_isPlaying = false;
        }
    }
}

}

