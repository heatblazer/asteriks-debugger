#ifndef UTILS_H
#define UTILS_H

namespace izdebug {

class utils {
    public:
    static bool aux_is_valid_ip_port(const char* fname);
    static int aux_strlen(const char* data);
    static void trim(char* data);

    };

} // namespace izdebug


#endif // UTILS_H
