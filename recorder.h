#ifndef RECORDER_H
#define RECORDER_H

#include <QObject>

// pjlib //
#include <pjmedia.h>
#include <pjlib.h>
#include <pjsua.h>

namespace izdebug {

// capture the frames to wav file,
// also debug the data
class Recorder : public QObject
{
    Q_OBJECT
public:
    explicit Recorder(QObject* parent=nullptr);
    virtual ~Recorder();
    bool create(const char* fname);
    void destroy(void);
    pjmedia_port* pjPort();

public slots:
    void stop();
    void start();

private:
    void _disconnect_and_remove();

private:

    pjmedia_port* p_port;
    unsigned m_slot;
    bool m_isOk;
    bool m_isRecording;

};


} // namespace izdebug


#endif // RECORDER_H
