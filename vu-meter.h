#ifndef VUMETER_H
#define VUMETER_H

#include <QObject> // parent
#include <math.h> // math

namespace izdebug {

class VuMeter : public QObject
{
#if 0
    Q_OBJECT
public:
    explicit VuMeter(QObject* parent=nullptr);
    virtual ~VuMeter();

private:
    void _vu_meter_on_sample(int channels, int count, float *buffer) {
        float K = 0;
        float sum = 0;
        float volume = 0;

        for(int i = 0; i < count; i++) {
            sum += pow(buffer[i], 2);
        }

        volume = 20 * log10(sqrt(sum / count)) + K;

        // printf("volume = % .02f\n", volume);
        // char *s[light_count] = "";

        const float origVolume = volume;

        // Make volume a number from 0 to number of dB
        volume += (light_count * lights_per_dB);
        volume = fmax(0, volume);
        volume /= lights_per_dB;
        int v = fmin(volume, light_count);
        printf("%6.2f %d %s\n", origVolume, v, stars + (light_count - v));
    }

private:
    static const float  m_start_threshold = -3;
    static const int    m_lights_per_dB = 3;
    static const int    m_light_count = 10;
#endif
};

// vu meter logic
#if 0
#include "vu-meter.h"

#include <stdio.h>


// unneded not a console app
static char stars[10 + 1]; // must match light_count
// uneended not a console app
void vu_meter_init() {
    int i;
    for(i = 0; i < light_count; i++) {
        stars[i] = '*';
    }
    stars[light_count] = 0;
}


// vim: set ts=8 sw=4 sts=4 expandtab:

#endif

} // namespace izdebug

#endif // VUMETER_H
