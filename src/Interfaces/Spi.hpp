#ifndef SPI
#define SPI

#include <nuttx/spi/spi.h>

#include "Base.hpp"

namespace spi {
constexpr size_t trans_buf_size = 1024;

class Interface : public interface::Base {
private:
    uint32_t m_frequency = 1000000;
    uint8_t m_spi_mode = SPIDEV_MODE0;
    uint8_t m_nbits = 8;
    uint8_t m_ntrans = 1;

private:
    uint8_t tx_trans_buf[trans_buf_size] = {0};
    uint8_t rx_trans_buf[trans_buf_size] = {0};

public:
    Interface(const char *name, const char *path, interface::Mode mode);
    ~Interface();

public:
    virtual int init() override;
    virtual int exec(const char *command) override;
    virtual ssize_t write(const void *data, size_t size) override;
    virtual ssize_t read(void *data, size_t size) override;
};
}  // namespace spi

#endif  // SPI
