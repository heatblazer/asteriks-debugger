#include "socket.h"
#include <string.h>

namespace izdebug {

UnixSocket::UnixSocket(const char *uri, const char *port)
{
    strncpy(m_sockets[CLIENT].name, uri, 256);
    memset(&m_sockets[CLIENT].socket, 0, sizeof(m_sockets[CLIENT].socket));
    strncpy(m_sockets[CLIENT].port, port, 64);

}

UnixSocket::~UnixSocket()
{

}

bool UnixSocket::init()
{
    bool res = _init_client() && _init_server();
    return res;
}


/// Omit for now
/// \brief UnixSocket::_init_server
/// \return
///
bool UnixSocket::_init_server()
{
#if 0
    m_sockets[SERVER].fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockets[SERVER].fd < 0) {
        return false;
    }

    m_sockets[SERVER].socket.sin_family = AF_INET;
    m_sockets[SERVER].socket.sin_addr.s_addr = INADDR_ANY;
    m_sockets[SERVER].socket.sin_port = htons(m_sockets[SERVER].port);

#endif
    return true;
}


/// register client function
/// \brief UnixSocket::_init_client
/// \return
///
bool UnixSocket::_init_client()
{

}

} // namespace izdebug
