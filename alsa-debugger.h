#ifndef ALSADEBUGGER_H
#define ALSADEBUGGER_H

#include <alsa/asoundlib.h>

// capture samples from the audio device

namespace izdebug {
class ALSAAbstract;
class ALSAPlayer;
class ALSARecorder;
class ALSADebugger;


class ALSAAbstract
{
public:
    ALSAAbstract();
    virtual ~ALSAAbstract();
    virtual bool init()=0;
    virtual void cleanup();

protected:
    snd_pcm_t*  p_handle;
    snd_pcm_hw_params_t* p_params;
};

class ALSAPlayer : public ALSAAbstract
{
public:
    ALSAPlayer();
    ~ALSAPlayer();
    bool init();

private:
    // custom cleanup
    void _cleanup();

private:

};

class ALSARecorder : public ALSAAbstract
{
public:
    ALSARecorder();
    ~ALSARecorder();
    bool init();

private:
    void _cleanup();
};

class ALSADebugger
{
public:
    ALSADebugger();
    ~ALSADebugger();

    bool create(bool has_player=true, bool has_rec=true);

private:
    ALSAAbstract*   p_player;
    ALSAAbstract*   p_recorder;
};


} // namespace izdebug

#endif // ALSADEBUGGER_H
