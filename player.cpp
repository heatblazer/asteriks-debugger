#include "player.h"

// TODO

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
    return _create(m_fname.toLatin1().constData());
}

bool Player::_create(const char *fname)
{

}

void Player::destroy()
{

}

pjmedia_port *Player::pjPort()
{

}

void Player::stop()
{

}

void Player::start()
{

}

void Player::_disconnect_and_remove()
{

}



} // namespace izdebug

