#ifndef RINGBUFFER_H
#define RINGBUFFER_H

namespace izdebug {

class RingBuffer
{
public:
    RingBuffer();
    RingBuffer(unsigned long size=0);
    ~RingBuffer();


private:
    unsigned long m_size;
    void* m_data;
};


} // namespace izdebug


#endif // RINGBUFFER_H
