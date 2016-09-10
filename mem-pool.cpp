#include "mem-pool.h"

namespace izdebug {

Pool* Pool::s_inst = nullptr;
static Pool g_pool;

Pool &Pool::Instance()
{
    return g_pool;
}

void *Pool::zero_alloc(pj_size_t size)
{
    return pj_pool_zalloc(p_pool, size);
}

void *Pool::alloc(pj_size_t size)
{
    return pj_pool_alloc(p_pool, size);
}

pj_pool_t *Pool::toPjPool() const
{
    return p_pool;
}

Pool::Pool()
{
    pj_caching_pool_init(&m_cpool, &pj_pool_factory_default_policy, 0);
    p_pool = pj_pool_create(&m_cpool.factory,
                            "DEBUG POOL",
                            4096,
                            4096,
                            NULL);

}

Pool::~Pool()
{

}


}
