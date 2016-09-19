#ifndef BRIDGEPORT_H
#define BRIDGEPORT_H

#include "mediaport.h"

namespace izdebug {

class BridgePort : public MediaPort{
public:
    BridgePort(pjmedia_port* put, pjmedia_port* get);
    ~BridgePort();

    bool create();
private:
    bool _create_bidirectional();

private:
    pjmedia_port* get_port;
    pjmedia_port* put_port;
};

} // namespace izdebug

#endif // BRIDGEPORT_H
