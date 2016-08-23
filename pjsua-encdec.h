#ifndef PJSUAENCDEC_H
#define PJSUAENCDEC_H

//Qt //
#include <QObject>

// pjsip //
#include <pjlib.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>



namespace izdebug {

class Encoder : public QObject
{
    Q_OBJECT
public:
    explicit Encoder(QObject* parent=nullptr);
    virtual ~Encoder();

};


class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(QObject* parent=nullptr);
    virtual ~Decoder();

};

} // namespace izdebug

#endif // PJSUAENCDEC_H
