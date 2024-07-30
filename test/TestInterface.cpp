#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "Base.hpp"
#include "Message.hpp"
#include "probe.h"

using namespace interface;

char test_file_1[] = "test_file_1.txt";
char test_file_2[] = "test_file_2.txt";

class TestInterface : public Base {
public:
    TestInterface(const char *name, const char *path, Mode mode)
        : Base(name, path, mode) {
    }

    ~TestInterface() {
        mq_close(this->src);
        close(this->fd);
    }

    int init() override {
        this->fd = open(this->m_path, O_RDWR);
        if (this->fd < 0) {
            syslog(LOG_ERR, "Failed to open %s in %s", this->m_name,
                   this->m_path);
            syslog(LOG_ERR, "%s", strerror(errno));
            return -1;
        }
        syslog(LOG_DEBUG, "%s: %s is open", this->m_name, this->m_path);

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = sizeof(Message);
        attr.mq_curmsgs = 0;
        this->src = mq_open(this->m_queue, O_RDWR | O_CREAT, 0666, &attr);
        if (this->src < 0) {
            syslog(LOG_ERR, "Failed to create queue for %s", this->m_name);
            syslog(LOG_ERR, "%s", strerror(errno));
            close(this->fd);
            return -1;
        }

        this->run();
        return 0;
    }

    int exec(const char *cmd) override {
        (void)cmd;
        return 0;
    }
};

TEST(InterfaceReadWrite) {
    openlog("InterfaceEssentials", LOG_PID | LOG_CONS, LOG_USER);
    char text[1024] =
        "Interface #1 writes to Interface #2 through message queue";

    // Remove tmp
    remove(test_file_1);
    remove(test_file_2);

    // Create files
    auto fd = open(test_file_1, O_RDWR | O_CREAT, 0777);
    write(fd, text, strlen(text) + 1);
    close(fd);

    fd = open(test_file_2, O_RDWR | O_CREAT, 0777);
    close(fd);

    // Init two interfaces
    Base *if1 = new TestInterface("tf1", test_file_1, READ_ONLY);
    Base *if2 = new TestInterface("tf2", test_file_2, WRITE_ONLY);
    if1->connect(if2);
    if1->init();
    if2->init();
    sleep(1);

    // Check that data matches
    char text_read[1024];
    fd = open(test_file_2, O_RDONLY);
    read(fd, text_read, 1024);
    CHECK_STR(text, text_read);

    remove(test_file_1);
    remove(test_file_2);
    closelog();
}
