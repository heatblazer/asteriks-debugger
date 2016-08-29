#include "player.h"

#include "mem-pool.h"
#include <pjsua-lib/pjsua_internal.h>

// gui //
#include "gui.h"

namespace izdebug {

Player::Player(const QString &fname, QObject *parent)
    : MediaPort(parent),
      m_fname(fname)
{

}

Player::~Player()
{

}

bool Player::create()
{
    // one time creation , don`t call multile times
    if(!m_isOk) {
        m_isOk = true;
        return _create(m_fname.toLatin1().constData());
    }
    return false;
}

bool Player::_create(const char *fname)
{

    pj_status_t status;
    status = pjmedia_wav_player_port_create(Pool::Instance().toPjPool(),
                                            fname,
                                            0,
                                            0,
                                            0,
                                            &p_port);
    if (status != PJ_SUCCESS) {
        return false;
    }

    connect(&m_timer, SIGNAL(timeout()),
            this, SLOT(hTimeout()));
    connect(this, SIGNAL(playing(bool)),
            this, SLOT(hPlaying(bool)), Qt::DirectConnection);



    return true;

}

void Player::destroy()
{

}

pjmedia_port *Player::pjPort()
{
    return p_port;

}

void Player::stop()
{
    if (m_isPlaying && m_isOk) {
        m_isPlaying = false;
        m_timer.stop();
    }

    emit playing(m_isPlaying);

}

void Player::play()
{
    if (!m_isPlaying && m_isOk) {
        m_isPlaying = true;
        m_timer.start();
    }

    emit playing(m_isPlaying);


}

void Player::setFile(const char *fname)
{
    if(!m_fname.isEmpty()) {
        m_fname.clear();
        m_fname.append(fname);
    }

}

void Player::hPlaying(bool s)
{
    if (s) {
        pjmedia_conf_add_port(pjsua_var.mconf, Pool::Instance().toPjPool(),
                              p_port, NULL, &m_slot);
        pjmedia_conf_connect_port(pjsua_var.mconf, 0, m_slot, 0);
        pjmedia_conf_connect_port(pjsua_var.mconf, m_slot, 0, 0);

    } else {
        pjmedia_conf_disconnect_port(pjsua_var.mconf, 0, m_slot);
    }

}

void Player::hTimeout()
{
    pjmedia_frame frame;
    pj_int16_t framebuf[640];
    unsigned ms;

    if(1){

        pjmedia_port_get_frame(p_port, &frame);

        pj_int32_t level32 = pjmedia_calc_avg_signal(framebuf,
                          PJMEDIA_PIA_SPF(&p_port->info));
        int level = pjmedia_linear2ulaw(level32) ^ 0xFF;
        for(int i=0; i < 100; ++i) {
            ms = i * 1000 * PJMEDIA_PIA_SPF(&p_port->info) /
                PJMEDIA_PIA_SRATE(&p_port->info);
            char txt[128]={0};
            sprintf(txt, "%03d.%03d\t%7d\t%7d\n",
                   ms/1000, ms%1000, level, level32);
            Console::Instance().putData(txt);
        }
    }

}


void Player::_disconnect_and_remove()
{
    if (m_isPlaying && m_isOk) {
        pj_status_t status = pjmedia_conf_disconnect_port(pjsua_var.mconf,
                                     0, m_slot);

        if (status == PJ_SUCCESS) {
            pjmedia_port_destroy(p_port);
            m_isOk = false; // we can create it again
        }
    }
}



} // namespace izdebug

