#ifndef SPI
#define SPI

#include "InterfaceBase.hpp"

constexpr const char SPI_DEFAULT_PATH[] = "/dev/spi0";
constexpr const char SPI_DEFAULT_NAME[] = "spi0";

class Spi : public interface::InterfaceBase {
public:
    Spi(const char *name, const char *path, interface::Mode mode);
    ~Spi();

public:
    virtual int init() override;
    virtual int exec(const char *cmd) override;
};

#endif  // SPI
