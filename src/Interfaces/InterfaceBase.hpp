#ifndef INTERFACE
#define INTERFACE

#include <mqueue.h>
#include <pthread.h>

namespace interface {
constexpr int name_max_length = 16;
constexpr int path_max_length = 32;
constexpr int queue_path_max_length = 48;
constexpr int max_interfaces = 8;
constexpr int buf_max_size = 1024;
enum Mode {
    READ_ONLY = 1,
    WRITE_ONLY = 2,
    READ_WRITE = 3
};

class InterfaceBase {
public:
    virtual int init() = 0;
    virtual int exec(const char *cmd) = 0;

public:
    InterfaceBase() = delete;
    InterfaceBase(const char *name, const char *path, Mode mode);
    virtual ~InterfaceBase() = 0;

protected:
    char m_name[name_max_length] = {0};
    char m_path[path_max_length] = {0};
    char m_queue[queue_path_max_length] = {0};
    Mode m_mode;
    pthread_t pthread_tx;
    pthread_t pthread_rx;
    int fd;
    mqd_t src;
    mqd_t dst[max_interfaces] = {0};
    int run();

public:
    static void *threadRx(void *arg);
    static void *threadTx(void *arg);

public:
    int connect(InterfaceBase *dst);
};
}  // namespace interface

#endif  // INTERFACE
