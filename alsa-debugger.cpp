#include "alsa-debugger.h"

// debug messages //
#include <iostream>

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
        p_player = new ALSAPlayer;
        if (!p_player) {
            ret &= false;
        }
    }

    if (has_rec) {
        p_recorder = new ALSARecorder;
        if(!p_recorder) {
            ret &= false;
        }
    }

    return ret;
}


} // namespace izdebug

