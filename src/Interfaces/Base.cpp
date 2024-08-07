#include "Base.hpp"

#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "Message.hpp"
#include "RingBuffer.hpp"

using namespace interface;

Base::Base(const char *name, const char *path, Mode mode)
    : m_mode(mode) {
    strncpy(this->m_name, name, name_max_length);
    strncpy(this->m_path, path, path_max_length);
    sprintf(this->m_queue, "/mq_%s", this->m_name);
}

Base::~Base() {
    if (this->fd > 0) {
        close(this->fd);
    }
    if (this->src > 0) {
        mq_close(this->src);
    }
    for (int i = 0; i < max_interfaces; ++i) {
        if (this->dst[i] > 0) {
            mq_close(this->dst[i]);
        }
    }
}

ssize_t Base::write(const void *data, size_t size) {
    return ::write(fd, data, size);
}
ssize_t Base::read(void *data, size_t size) {
    return ::read(fd, data, size);
}

void *Base::threadTx(void *arg) {
    Base *inst = reinterpret_cast<Base *>(arg);

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
        while (static_cast<unsigned>(rb) < msg.size) {
            rb += msg.buf->read(inst->tx_buf, msg.size);
        }

        if (rb <= 0) {
            if (rb < 0)
                syslog(LOG_ERR, "%s: failed to get data from buf",
                       inst->m_name);
            continue;
        }

        syslog(LOG_DEBUG, "%s: received %ld bytes from queue", inst->m_name,
               rb);

        ssize_t wb = 0;
        while (wb < rb) {
            wb += inst->write(inst->tx_buf, rb);
        }

        if (wb < 0) {
            syslog(LOG_ERR, "Failed to write data by %s", inst->m_name);
            continue;
        }
        syslog(LOG_DEBUG, "%s: wrote %ld bytes", inst->m_name, wb);
    }
}

void *Base::threadRx(void *arg) {
    Base *inst = reinterpret_cast<Base *>(arg);

    RingBuffer *ring_buf[max_interfaces];
    Message *msg[max_interfaces];

    for (int i = 0; i < max_interfaces; ++i) {
        ring_buf[i] = new RingBuffer(buf_max_size);
    }

    for (int i = 0; i < max_interfaces; ++i) {
        msg[i] = new Message;
        msg[i]->buf = ring_buf[i];
    }

    while (1) {
        unsigned int prio = 1;
        ssize_t rb = inst->read(inst->rx_buf, buf_max_size);
        if (rb <= 0) {
            if (rb < 0)
                syslog(LOG_ERR, "%s: failed to receive data", inst->m_name);
            continue;
        }
        syslog(LOG_DEBUG, "%s: received %ld bytes", inst->m_name, rb);

        for (int i = 0; i < max_interfaces; ++i) {
            if (inst->dst[i] > 0) {
                ssize_t wb = 0;
                while (wb < rb) {
                    wb += msg[i]->buf->write(inst->rx_buf, rb);
                }
                msg[i]->size = rb;

                ssize_t wbm =
                    mq_send(inst->dst[i], reinterpret_cast<char *>(msg[i]),
                            sizeof(Message), prio);
                if (wbm < 0) {
                    syslog(LOG_ERR, "%s: failed to write data", inst->m_name);
                    continue;
                }
                syslog(LOG_DEBUG, "%s: wrote %ld bytes to queue", inst->m_name,
                       wb);
            }
        }
    }
}

int Base::connect(Base *cons) {
    for (int i = 0; i < max_interfaces; ++i) {
        if (this->dst[i] == 0) {
            this->dst[i] = mq_open(cons->m_queue, O_WRONLY);
            if (this->dst[i] < 0) {
                syslog(LOG_ERR, "Failed to connect queue for %s", this->m_name);
                syslog(LOG_ERR, "%s", strerror(errno));
                close(this->fd);
                return -1;
            }
            syslog(LOG_DEBUG, "%s: connected to %s", this->m_name,
                   cons->m_queue);
            return 0;
        }
    }
    syslog(LOG_ERR, "Failed to connect device %s to %s", cons->m_name,
           this->m_name);
    return -1;
}

int Base::run() {
    if (this->m_mode == WRITE_ONLY || this->m_mode == READ_WRITE) {
        pthread_attr_t tx_attr = {0};
        pthread_attr_init(&tx_attr);
        pthread_attr_setstacksize(&tx_attr, 4096);
        pthread_create(&this->pthread_tx, &tx_attr, this->threadTx,
                       static_cast<Base *>(this));
    }
    if (this->m_mode == READ_ONLY || this->m_mode == READ_WRITE) {
        pthread_attr_t rx_attr = {0};
        pthread_attr_init(&rx_attr);
        pthread_attr_setstacksize(&rx_attr, 4096);
        pthread_create(&this->pthread_rx, &rx_attr, this->threadRx,
                       static_cast<Base *>(this));
    }

    return 0;
}
