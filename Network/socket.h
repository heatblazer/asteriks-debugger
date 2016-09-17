#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

namespace izdebug {

class UnixSocket
{
    enum SocketType {CLIENT, SERVER, SIZE};
public:
    explicit UnixSocket(const char* uri, const char* port);
    ~UnixSocket();
    bool init(void);

private:
    bool _init_server();
    bool _init_client();
private:
    struct {
        int  fd;
        char port[64];
        char name[256];
        struct sockaddr_in socket;
        struct hostent* server; // for clients
    } m_sockets[SIZE];

};

} // namespace izdebug


#endif // SOCKET_H
