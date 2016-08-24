#ifndef RECORDER_H
#define RECORDER_H

#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

namespace izdebug {

class Recorder
{

public:
    explicit Recorder();
    ~Recorder();
    bool create(const char* fname, bool is_stream=false);
    pjmedia_port* pjPort();
private:

    pjmedia_port* p_port;
    int m_id;

};


} // namespace izdebug


#endif // RECORDER_H
