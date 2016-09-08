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
    m_timer.setInterval(10); // get a frame each 10 ms

    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(hTimeout1()));
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
#if 0
        pjmedia_snd_port_create_player(Pool::Instance().toPjPool(),
                                    -1,
                                    PJMEDIA_PIA_SRATE(&conf->info),
                                    PJMEDIA_PIA_CCNT(&conf->info),
                                    PJMEDIA_PIA_SPF(&conf->info),
                                    PJMEDIA_PIA_BITS(&conf->info),
                                    0,
                                    &p_sndPort);
#endif
         {

//            pjmedia_snd_port_connect(p_sndPort, p_port);
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
//    pj_status_t s1 = pjmedia_conf_connect_port(pjsua_var.mconf, 0, m_slot, 0);
//    pj_status_t s2 = pjmedia_conf_connect_port(pjsua_var.mconf, 0, m_slot, 0);


    if(!m_isPlaying) {
        m_isPlaying = true;
        m_timer.start();
    }
    emit update(m_isPlaying);
}

void Player::stop()
{
//    pj_status_t s1 = pjmedia_conf_disconnect_port(pjsua_var.mconf, 0, m_slot);
//    pj_status_t s2 = pjmedia_conf_disconnect_port(pjsua_var.mconf, 0, m_slot);

    if(m_isPlaying) {
        m_isPlaying = false;
        m_timer.stop();
    }
    emit update(m_isPlaying);
}


void Player::hTimeout1()
{
    // handle the tick
    std::cout << "Handle tick " << std::endl;

}

void Player::_disconnect_and_remove()
{
    if (m_isOk) {
        pj_status_t status = pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     0, m_slot);

        if (status == PJ_SUCCESS) {
            pjmedia_port_destroy(p_port);
            m_isOk = false; // we can create it again
            m_isPlaying = false;
        }
    }
}



}

