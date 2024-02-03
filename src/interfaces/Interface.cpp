#include "Interface.hpp"

#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"
#include "RingBuffer.hpp"

Interface::~Interface() {}

constexpr int IF_BUF_MAX_SIZE = 1024;

void *Interface::tx_thread(void *arg) {
    Interface *inst = reinterpret_cast<Interface *>(arg);

    char buf[IF_BUF_MAX_SIZE];

    while (1) {
        unsigned int prio;
        Message msg;
        ssize_t rbm = mq_receive(inst->src, reinterpret_cast<char *>(&msg),
                                 sizeof(Message), &prio);
        if (rbm < 0) {
            syslog(LOG_ERR, "%s: failed to receive msg from queue",
                   inst->m_name);
            continue;
        }

        ssize_t rb = 0;
        while (rb < msg.size) {
            rb += msg.buf->read(buf, msg.size);
        }

        if (rb < 0) {
            syslog(LOG_ERR, "%s: failed to get data from buf", inst->m_name);
            continue;
        }

        if (inst->m_verbosity) {
            syslog(LOG_INFO, "%s: received %d bytes from queue", inst->m_name,
                   rb);
        }


        ssize_t wb = 0;
        while (wb < rb) {
            wb += write(inst->fd, buf, rb);
        }

        if (wb < 0) {
            syslog(LOG_ERR, "Failed to write data by %s", inst->m_name);
            continue;
        }
        if (inst->m_verbosity) {
            syslog(LOG_INFO, "%s: wrote %d bytes", inst->m_name, wb);
        }
    }
}

void *Interface::rx_thread(void *arg) {
    Interface *inst = reinterpret_cast<Interface *>(arg);

    RingBuffer *ring_buf[IF_MAX_INTERFACES];
    Message *msg[IF_MAX_INTERFACES];

    for (int i = 0; i < IF_MAX_INTERFACES; ++i) {
        ring_buf[i] = new RingBuffer(IF_MAX_INTERFACES);
    }

    for (int i = 0; i < IF_MAX_INTERFACES; ++i) {
        msg[i] = new Message;
        msg[i]->buf = ring_buf[i];
    }

    char buf[IF_BUF_MAX_SIZE] = {0};

    while (1) {
        unsigned int prio = 1;
        ssize_t rb = read(inst->fd, buf, IF_BUF_MAX_SIZE);
        if (rb < 0) {
            syslog(LOG_ERR, "%s: failed to receive data", inst->m_name);
            continue;
        }
        if (inst->m_verbosity) {
            syslog(LOG_INFO, "%s: received %d bytes", inst->m_name, rb);
        }

        for (int i = 0; i < IF_MAX_INTERFACES; ++i) {
            if (inst->dst[i] > 0) {
                ssize_t wb = 0;
                while (wb < rb) {
                    wb += msg[i]->buf->write(buf, rb);
                }
                msg[i]->size = rb;

                ssize_t wbm =
                    mq_send(inst->dst[i], reinterpret_cast<char *>(msg[i]),
                            sizeof(Message), prio);
                if (wbm < 0) {
                    syslog(LOG_ERR, "%s: failed to write data", inst->m_name);
                    continue;
                }
                if (inst->m_verbosity) {
                    syslog(LOG_INFO, "%s: wrote %d bytes to queue",
                           inst->m_name, wb);
                }
            }
        }
    }
}

int Interface::connect(Interface *src_dev) {
    for (int i = 0; i < IF_MAX_INTERFACES; ++i) {
        if (src_dev->dst[i] == 0) {
            src_dev->dst[i] = mq_open(src_dev->m_queue, O_WRONLY);
            if (this->m_verbosity) {
                syslog(LOG_INFO, "%s: connected to %s", this->m_name,
                       src_dev->m_queue);
            }
            return 0;
        }
    }
    syslog(LOG_ERR, "Failed to connect device %s to %s", src_dev->m_name,
           this->m_name);
    return -1;
}
