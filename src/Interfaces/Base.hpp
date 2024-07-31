#ifndef INTERFACE
#define INTERFACE

#include <mqueue.h>
#include <pthread.h>

#include "RingBuffer.hpp"

namespace interface {
constexpr int name_max_length = 16;
constexpr int path_max_length = 32;
constexpr int queue_path_max_length = 48;
constexpr int max_interfaces = 8;
constexpr int buf_max_size = 256;
enum Mode { READ_ONLY = 1, WRITE_ONLY = 2, READ_WRITE = 3 };

class Base {
public:
    virtual int init() = 0;
    virtual int configure(const char *command);
    virtual ssize_t write(const void *data, size_t size);
    virtual ssize_t read(void *data, size_t size);

public:
    const char *name();

public:
    Base() = delete;
    Base(const char *name, const char *path, Mode mode);
    virtual ~Base() = 0;

protected:
    char m_name[name_max_length] = {0};
    char m_path[path_max_length] = {0};
    char m_data_queue[queue_path_max_length] = {0};
    Mode m_mode;
    pthread_t pthread_tx;
    pthread_t pthread_rx;
    pthread_t pthread_cmd;
    int fd;
    mqd_t src;
    mqd_t dst[max_interfaces] = {0};
    int run();
    virtual int exec(const char *cmd) = 0;

    RingBuffer *rx_ring_buf[max_interfaces];
    RingBuffer *cmd_ring_buf;

protected:
    char tx_buf[buf_max_size] = {0};
    char rx_buf[buf_max_size] = {0};
    char cmd_buf[buf_max_size] = {0};

public:
    static void *threadRx(void *arg);
    static void *threadTx(void *arg);
    static void *threadCmd(void *arg);

public:
    int connect(Base *dst);
};
}  // namespace interface

#endif  // INTERFACE
