#ifndef ALSADEBUGGER_H
#define ALSADEBUGGER_H


// capture samples from the audio device

namespace izdebug {
class ALSAPlayer;
class ALSARecorder;
class ALSADebugger;

class ALSAPlayer
{
public:
    ALSAPlayer();
    ~ALSAPlayer();
    bool init();
};

class ALSARecorder
{
public:
    ALSARecorder();
    ~ALSARecorder();
    bool init();


};

class ALSADebugger
{
public:
    ALSADebugger();
    ~ALSADebugger();

    bool create(bool has_player=true, bool has_rec=true);



private:
    ALSAPlayer*   p_player;
    ALSARecorder* p_recorder;
};


} // namespace izdebug

#endif // ALSADEBUGGER_H
