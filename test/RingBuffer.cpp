#include "RingBuffer.hpp"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <syslog.h>
#include <unistd.h>

#include "probe.h"

char tx_buf[] = {'0', '1', '2', '3', '4', '5'}; 
char rx_buf[1024] = {0}; 
RingBuffer ringbuf(2);

TEST(RingBufBasics) {
    ringbuf.write(tx_buf, 1);
    ringbuf.read(rx_buf, 1);   
    CHECK_EQ(rx_buf[0], tx_buf[0]);
}

TEST(RingBufMultipleWriteRead1) {
    ringbuf.write(tx_buf, 2);
    ringbuf.read(rx_buf, 2);   
    CHECK_EQ(rx_buf[0], tx_buf[0]);
    CHECK_EQ(rx_buf[1], tx_buf[1]);
}

TEST(RingBufMultipleWriteRead2) {
    ringbuf.write(tx_buf, 2);
    ringbuf.read(rx_buf, 1);   
    CHECK_EQ(rx_buf[0], tx_buf[0]);
    ringbuf.read(rx_buf, 1);   
    CHECK_EQ(rx_buf[0], tx_buf[1]);
}

void *writer_thread(void *arg) {
    (void)arg;
    ringbuf.write(&tx_buf[0], 2);
    CHECK_TRUE(ringbuf.full());
    sleep(2);
    CHECK_TRUE(ringbuf.empty());
    ringbuf.write(&tx_buf[3], 2);
    pthread_exit(NULL);
}

void *reader_thread(void *arg) {
    (void)arg;
    sleep(1);
    CHECK_TRUE(ringbuf.full());
    ringbuf.read(&rx_buf[0], 2);
    CHECK_EQ(rx_buf[0], tx_buf[0]);
    CHECK_EQ(rx_buf[1], tx_buf[1]);
    ringbuf.read(&rx_buf[0], 2);
    CHECK_EQ(rx_buf[0], tx_buf[3]);
    CHECK_EQ(rx_buf[1], tx_buf[4]);
    pthread_exit(NULL);
}

TEST(RingBufMultithreading) {
    pthread_t writer;
    pthread_t reader;
    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    sleep(1);
}
