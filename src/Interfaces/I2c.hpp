#ifndef I2C
#define I2C

#include <nuttx/i2c/i2c_master.h>

#include "Base.hpp"

namespace i2c {
constexpr size_t trans_buf_size = 1024;

class Interface : public interface::Base {
private:
    uint32_t m_frequency = 100000;
    uint8_t m_address = 0xAA;
    uint8_t m_addrlen = 7;

private:
    uint8_t tx_trans_buf[trans_buf_size] = {0};
    uint8_t rx_trans_buf[trans_buf_size] = {0};

public:
    Interface(const char *name, const char *path, interface::Mode mode);
    ~Interface();

public:
    virtual int init() override;
    virtual int exec(const char *cmd) override;
    virtual ssize_t write(const void *data, size_t size) override;
    virtual ssize_t read(void *data, size_t size) override;
};
}  // namespace i2c

#endif  // I2C
