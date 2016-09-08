#ifndef TONEGEN_H
#define TONEGEN_H

#include "mediaport.h"

namespace izdebug {

class ToneGen: public MediaPort
{
    Q_OBJECT
public:
    static pj_status_t cbGetFrame(pjmedia_port* port, pjmedia_frame* frm);
public:
    explicit ToneGen(unsigned srate, unsigned chans, QObject* parent=nullptr);
    virtual ~ToneGen();
    bool create();

private:
    bool _create_sine_port(unsigned sampling_rate, unsigned channel_count);
private:
    pjmedia_snd_port* p_sndPort;

    unsigned m_srate;
    unsigned m_chans;

};

}

#endif // TONEGEN_H
