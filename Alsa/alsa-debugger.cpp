#include "alsa-debugger.h"

// debug messages //
#include <iostream>

// ansi C //
#include <stdio.h>
#include <stdlib.h>

namespace izdebug {

ALSAAbstract::ALSAAbstract()
    : p_handle(nullptr),
      p_params(nullptr)
{

}

ALSAAbstract::~ALSAAbstract()
{
    std::cout << "~ALSAAbstrace" << std::endl;
}

void ALSAAbstract::cleanup()
{
    // cleanup TODO
}

ALSAPlayer::ALSAPlayer():
    ALSAAbstract()
{

}

ALSAPlayer::~ALSAPlayer()
{
    std::cout << "~ALSAPlayer" << std::endl;
    _cleanup();
}

bool ALSAPlayer::init()
{
    int err=0;
    if ((err = snd_pcm_open(&p_handle, NULL, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf(stderr, "cannot open audio device: %s :(%s)\n",
                "NULL", snd_strerror(err));

        return false;
    }

    if ((err = snd_pcm_hw_params_malloc(&p_params)) < 0) {
        fprintf(stderr, "cannoc allocate hardware params (%s)\n",
                snd_strerror(err));

        return false;

    }

    if ((err = snd_pcm_hw_params_any(p_handle, p_params)) < 0) {
        fprintf(stderr, "cannot init hw structure params (%s)\n",
                snd_strerror(err));

        return false;
    }

    if((err = snd_pcm_hw_params_set_access(p_handle,
                                           p_params,
                                           SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf(stderr, "cannot set acces (%s)\n",
                snd_strerror(err));

        return false;
    }

    // TODO: pass the init params for PCMA PCMU
    if((err = snd_pcm_hw_params_set_format(p_handle,
                                           p_params,
                                           SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf(stderr, "cannot set sample fmt (%s)\n",
                snd_strerror(err));

        return false;
    }


    return true;
}



void ALSAPlayer::_cleanup()
{
    // clean stuff then call

    ALSAAbstract::cleanup();
}

ALSARecorder::ALSARecorder()
    : ALSAAbstract()
{

}

ALSARecorder::~ALSARecorder()
{
    std::cout << "~ALSARecorder" << std::endl;
    _cleanup();
}

bool ALSARecorder::init()
{

}

void ALSARecorder::_cleanup()
{
    // clean your stuff then call
    ALSAAbstract::cleanup();
}

ALSADebugger::ALSADebugger()
    : p_player(nullptr),
      p_recorder(nullptr)
{

}

ALSADebugger::~ALSADebugger()
{
    if (p_player) {
        delete p_player;
    }

    if (p_recorder) {
        delete p_recorder;
    }
}

bool ALSADebugger::create(bool has_player, bool has_rec)
{
    bool ret = true;

    if (has_player) {
        p_player = new ALSAPlayer();
        if (!p_player) {
            ret &= false;
        } else {
            if (!p_player->init()) {
                ret &= false;
            }
        }
    }

    if (has_rec) {
        p_recorder = new ALSARecorder();
        if(!p_recorder) {
            ret &= false;
        }
    }

    return ret;
}


} // namespace izdebug

