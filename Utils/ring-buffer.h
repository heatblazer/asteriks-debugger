#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <string.h>
#include <iostream>

#include "Concurent/pjsua-thread.h"

namespace izdebug {
// lock guard //
template <class T> class LockGuard
{
public:
    explicit LockGuard(T& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~LockGuard()
    {
        m_mutex.unlock();
    }
private:
    T& m_mutex;
};



template <class T> class Queue
{
    struct item_t {
        T data;
        item_t* next;
    };

public:
    Queue() : m_head(nullptr), m_tail(nullptr), m_size(0)
    {
    }

    ~Queue()
    {
        _cleanup();
    }


    T   pop()
    {
        item_t* rem = m_head;
        T ret = rem->data;
        m_head = m_head->next;
        delete rem;
        m_size--;
        return ret;
    }

    void append(const T data)
    {

        if (m_head == nullptr) {
            m_head = (m_tail = new_item(data, m_head));
            return;
        }
        m_tail->next = new_item(data, m_tail->next);
        m_tail = m_tail->next;
    }


    unsigned int toArray(T** ret)
    {
        item_t* it = m_head;
        T* arr = new T[m_size];
        T* ait = arr;
        memset(arr, 0, m_size);
        while (it != NULL) {
            *ait++ = it->data;
            it = it->next;
        }
        for(int i=0; i < m_size; i++) {
            std::cout << "[" << arr[i] << "]";
        }
        std::cout << "\n";
        (*ret) = arr;
        return m_size; // arr size for iterratuon
    }

private:

    item_t* new_item(const T data, item_t* link)
    {
        item_t* i = new item_t;
        i->data = data;
        i->next = link;
        m_size++;
        return i;
    }

    void _cleanup()
    {
        for(item_t* it = m_head; it != NULL; )
        {
            item_t* tmp = it;
            it = it->next;
            delete tmp;
        }
    }

private:
    item_t* m_head;
    item_t* m_tail;
    unsigned int m_size;
};

/** Track based buffer, can read into 2D tracks
 * for optimized RW operations
 */
template <class T> class RingBuffer
{
public:
    explicit RingBuffer(int size, int tracks)
        : m_size(size), m_tracks(tracks)
    {
#ifdef HAS_TRACKS
        m_buff = new T*[m_tracks]; // n tracks
        for(int i=0; i < size; i++) {
            m_buff[i] = new T[m_size]; // allocate the tracks
        }
#else
        m_buff = new T[m_size];
#endif
        memset(m_buff, 0, 10);

        RW.read_head = RW.write_head = &m_buff[0];

        m_head = &m_buff[0];
        m_tail = &m_buff[m_size-1];

    }

    ~RingBuffer()
    {
        if (m_buff) {
            delete m_buff;
            m_buff = nullptr;
        }
    }

    void deleteme()
    {
        LockGuard<Mutex> lck(m_mutex);
        for(int i=0; i < m_size; i++) {
            std::cout << "[" << m_buff[i] << "]";
        }
    }

    void write(const T data)
    {
        LockGuard<Mutex> lck(m_mutex);
        (*RW.write_head) = data;
        _advance_write_head();
    }

    void write(const T* data, const int count)
    {
        LockGuard<Mutex> lck(m_mutex);
        for(int i=0; i < count; i++) {
            *RW.write_head = data[i];
            _advance_write_head();
        }
    }

    void read(T *read_data)
    {
        LockGuard<Mutex> lck(m_mutex);
        *read_data = *RW.read_head;
        _advance_read_head();

    }

    void read(T* read_data, const int count)
    {
        LockGuard<Mutex> lck(m_mutex);
        for(int i=0; i < count; i++) {
            read_data[i] = *RW.read_head;
            _advance_read_head();
        }
    }

private:
    void _advance_write_head()
    {
        if (RW.write_head != m_tail) {
            RW.write_head++;
        } else {
            RW.write_head = (T*)m_head;
        }
    }

    void _advance_read_head()
    {
        if (RW.read_head != m_tail) {
            RW.read_head++ ;
        } else {
            RW.read_head = (T*) m_head;
        }
    }


    /// read/write head difference
    /// negative - read head is ahead
    /// null - they are at the same point
    /// positive - write head is ahead
    /// \brief _RWDifference
    /// \return
    ///
    int _RWDifference()
    {
        return (RW.write_head - RW.read_head);
    }

    int _busy_blockR()
    {
        // [0][0][0][RW.w][0][0][0]
        // <---busy---> index
        int busy_index = (RW.read_head - m_head);
        return busy_index;
    }

    int _busy_blockW()
    {
        // [0][0][0][RW.w][0][0][0]
        // <---busy---> index
        int busy_index = (RW.write_head - m_head);
        return busy_index;
    }

private:
    struct {
        T* write_head;
        T* read_head;
    } RW;


    int m_size;
    int m_tracks;
    const T*   m_head;
    const T*   m_tail;

    // a two dimennsional array of buffers
    //  RW->readHead---> (1 track)[0][0][0][0][1][1]....
    //  RW->writeHead--> (2 track)[1][1][1][0][0][0]....
    //  RW->readHead---> (2 track)[1][1][1][0][0][0]....
    //  RW->writeHead--> (3 track)[0][0][0][1][1][1]....
    //  ...................................... etc.
    // so we don`t stick for reading and writing
    // in one dimension, a simple emulation of
    // a hdd with two heads and many tracks
#ifdef HAS_TRACKS
    T** m_buff;
#else
    T* m_buff;
#endif
    PjMutex m_mutex;

};

} // namespace izdebug

#endif // RINGBUFFER_H
