#ifndef SPI
#define SPI

#include "Interface.hpp"

constexpr const char SPI_DEFAULT_PATH[] = "/dev/spi0";
constexpr const char SPI_DEFAULT_NAME[] = "spi0";

class Spi : public Interface {
public:
    Spi();
    Spi(const char *name, const char *path, int verbosity);
    ~Spi();

public:
    virtual int init() override;
    virtual int exec(char *cmd) override;
};

#endif  // SPI
