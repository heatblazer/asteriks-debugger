#include "utils.h"


namespace izdebug {

/// Poorman algorithm for simple IP:PORT validation
/// \brief aux_is_valid_ip_port
/// \param fname
/// \return
///
bool utils::aux_is_valid_ip_port(const char* fname)
{
    int dots[4] = {0};
    int* didx = dots;
    int arrindex = 0;
    const char* it = fname;
    while (*it != '\0') {
        if (*it == '.') {
            (*didx) = arrindex;
            didx++;
        }
        if (*it == ':') {
            (*didx) = arrindex;
            didx++;
        }
        it++;
        arrindex++;
    }


    bool res = true;
    // first check if there are 0 indexes, should not be possible
    // can`t have a leading . in IP:PORT strings
    for(int i=0; i < 4; i++) {
        if (dots[i]==0) {
            res = false;
            return res; // no need for more checks - it`s not OK anyway
        }
    }

    for(int i; i < 4; i++) {
        for(int j=i+1; j < 4; j++) {
            if (dots[j] - dots[i] <= 1) {
                // they are adjacent no way to be a valid IP:PORT
                res &= false;
            }
        }
    }


    return res;
}


/// aux function to determine strlen
/// \brief aux_strlen
/// \param data
/// \return
///
int utils::aux_strlen(const char* data)
{
    int len = 0;
    while(*data != '\0') {
        len++;
        data++;
    }

    return len;
}

/// aux function to trim whitespaces (no tabs or newlines)
/// \brief trim
/// \param data
///
void utils::trim(char* data)
{
    char* it = data;
    char* begin= data;
    char* end = &data[aux_strlen(data)-1];
    int len = aux_strlen(data) - 1;
    int flush_idx = 0;

    while(*begin == ' ') {
        begin++;
    }

    while(*end == ' ' && end != begin) {
        *end = 0;
        end--;
    }
    end++;

    while(begin != end ) {
        *it++ = *begin++;
        flush_idx++;
    }
    // flush all
    for(int i=flush_idx; i < len; i++) {
        data[i] = 0;
    }

}


} // izdebug
