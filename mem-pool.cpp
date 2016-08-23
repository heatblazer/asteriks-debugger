#include "mem-pool.h"

namespace izdebug {

Pool* Pool::s_inst = nullptr;

Pool &Pool::Instance()
{
    if(s_inst == nullptr) {
        s_inst = new Pool();
    }

    return *s_inst;
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
