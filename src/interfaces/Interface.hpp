#ifndef INTERFACE
#define INTERFACE

#include <mqueue.h>

constexpr int IF_PATH_MAX_LENGTH = 32;
constexpr int IF_QUEUE_PATH_MAX_LENGTH = 48;
constexpr int IF_NAME_MAX_LENGTH = 16;
constexpr int IF_MAX_INTERFACES = 8;

class Interface {
public:
    virtual int init() = 0;
    virtual int exec(char *cmd) = 0;

public:
    virtual ~Interface() = 0;

protected:
    char m_name[IF_NAME_MAX_LENGTH] = {0};
    char m_path[IF_PATH_MAX_LENGTH] = {0};
    char m_queue[IF_QUEUE_PATH_MAX_LENGTH] = {0};
    int fd;
    mqd_t src;
    mqd_t dst[IF_MAX_INTERFACES] = {0};
    int m_verbosity;

public:
    static void *tx_thread(void *arg);
    static void *rx_thread(void *arg);

public:
    int connect(Interface *dst);
};

#endif  // INTERFACE
