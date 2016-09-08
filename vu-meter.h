#ifndef VUMETER_H
#define VUMETER_H

#include <QObject> // parent

// pjmedia lib //
#include <pjmedia.h>

namespace izdebug {

class VuMeter : public QObject
{
    // it`s emitter
    Q_OBJECT
public:
    explicit VuMeter(QObject* parent=nullptr);
    virtual ~VuMeter();

signals:

private:
    void _vu_meter_on_sample(pjmedia_port* port, int samples);


private:

};


} // namespace izdebug

#endif // VUMETER_H
