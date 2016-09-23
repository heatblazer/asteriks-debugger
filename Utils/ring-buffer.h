#ifndef RINGBUFFER_H
#define RINGBUFFER_H

namespace izdebug {

template <class T> class RingBuffer
{
public:
    explicit RingBuffer(int size, int tracks)
        : m_size(size),
          m_tracks(tracks)
    {
        m_data = new T*[m_tracks];
        for(int i=0; i < m_tracks; i++) {
            m_data = new T[m_size];
        }
    }

    ~RingBuffer();


private:
    int m_size;
    int m_tracks;
    int m_count;
    T** m_data; // normally uint8 but I have to use 16 bit integers for samples
};


} // namespace izdebug


#endif // RINGBUFFER_H
