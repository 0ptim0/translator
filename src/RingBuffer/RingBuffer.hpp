#ifndef RINGBUFFER
#define RINGBUFFER

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class RingBuffer {
private:
    char* buffer;
    size_t m_size;
    int head;
    int tail;
    pthread_mutex_t mtx;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;

public:
    RingBuffer(int size = 1024) : m_size(size), head(0), tail(0) {
        buffer = new char[m_size];
        pthread_mutex_init(&mtx, NULL);
        pthread_cond_init(&not_full, NULL);
        pthread_cond_init(&not_empty, NULL);
    }

    RingBuffer(const RingBuffer& other)
        : m_size(other.m_size), head(other.head), tail(other.tail) {
        buffer = new char[m_size];
        memcpy(buffer, other.buffer, m_size);
        pthread_mutex_init(&mtx, NULL);
        pthread_cond_init(&not_full, NULL);
        pthread_cond_init(&not_empty, NULL);
    }

    RingBuffer& operator=(const RingBuffer& other) {
        if (this != &other) {
            delete[] buffer;
            m_size = other.m_size;
            head = other.head;
            tail = other.tail;
            buffer = new char[m_size];
            memcpy(buffer, other.buffer, m_size);
        }
        return *this;
    }

    ~RingBuffer() {
        delete[] buffer;
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&not_full);
        pthread_cond_destroy(&not_empty);
    }

    ssize_t write(const char* buf, size_t size) {
        if (size == 0) {
            return 0;
        }

        if (size > this->m_size) {
            return -1;
        }

        pthread_mutex_lock(&mtx);

        while (freeSpace() < size) {
            pthread_cond_wait(&not_full, &mtx);
        }

        for (size_t i = 0; i < size; ++i) {
            buffer[tail] = buf[i];
            tail = (tail + 1) % this->m_size;
        }

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mtx);

        return size;
    }

    ssize_t read(char* buf, size_t size) {
        if (size == 0) {
            return 0;
        }

        pthread_mutex_lock(&mtx);

        while (head == tail) {
            pthread_cond_wait(&not_empty, &mtx);
        }

        size_t max_size = this->m_size - freeSpace();
        max_size = size < max_size ? size : max_size;

        for (size_t i = 0; i < max_size; ++i) {
            buf[i] = buffer[head];
            head = (head + 1) % this->m_size;
        }

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mtx);

        return max_size;
    }

    size_t freeSpace() const { return (head - tail - 1 + m_size) % m_size; }
};

#endif  // RINGBUFFER
