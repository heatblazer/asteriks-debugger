#include "player.h"

#include "mem-pool.h"
#include <pjsua-lib/pjsua_internal.h>

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

    connect(this, SIGNAL(playing(bool)),
            this, SLOT(hPlaying(bool)), Qt::DirectConnection);
    return true;

}

void Player::destroy()
{

}

pjmedia_port *Player::pjPort()
{

}

void Player::stop()
{
    if (m_isPlaying && m_isOk) {
        m_isPlaying = false;
    }

    emit playing(m_isPlaying);

}

void Player::play()
{
    if (!m_isPlaying && m_isOk) {
        m_isPlaying = true;
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

void Player::_disconnect_and_remove()
{

}



} // namespace izdebug

