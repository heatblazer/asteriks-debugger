#ifndef PLAYER_H
#define PLAYER_H

#include "mediaport.h"

#include <QTimer>

namespace izdebug {

class Player : public MediaPort
{
    Q_OBJECT
public:
    explicit Player(const QString& fname, QObject* parent=nullptr);
    virtual ~Player();
    bool create();
    void play();
    void stop();

signals:
    void update(bool s);
    void sendRxTx(unsigned rx, unsigned tx);

public slots:
    void test();

private:
    void _disconnect_and_remove();
private:
    pjmedia_snd_port* p_sndPort;
    QString m_fname;
    bool    m_isPlaying;

};

} // namespace izdebug
#endif // PLAYER_H
