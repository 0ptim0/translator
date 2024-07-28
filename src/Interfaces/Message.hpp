#ifndef MESSAGE
#define MESSAGE

#include "RingBuffer.hpp"

struct Message {
    RingBuffer *buf;
    unsigned int size;
};

#endif // MESSAGE
