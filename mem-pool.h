#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <pjlib.h>

namespace izdebug {

class Pool
{
public:
    static Pool& Instance();

    void* zero_alloc(pj_size_t size);
    void* alloc(pj_size_t size);
    pj_pool_t* toPjPool() const;

public:
     Pool();
    ~Pool();

    pj_pool_t*  p_pool;
    pj_caching_pool m_cpool;

    static Pool* s_inst;
};


} // namespace izdebug


#endif // MEMPOOL_H
