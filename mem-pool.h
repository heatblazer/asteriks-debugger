#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <pjlib.h>

namespace izdebug {

class Pool
{
public:
    static Pool& Instance();
    pj_pool_t* toPjPool() const;

private:
    Pool();
    ~Pool();

    pj_pool_t*  p_pool;
    pj_caching_pool m_cpool;

    static Pool* s_inst;
};


} // namespace izdebug


#endif // MEMPOOL_H
