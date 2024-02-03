#ifndef SPI
#define SPI

#include "Interface.hpp"

constexpr int SPI_MAX_PATH_LENGTH = 32;
constexpr const char SPI_DEFAULT_PATH[] = "/dev/spi0";

class Spi : public Interface {
private:
    char path[SPI_MAX_PATH_LENGTH] = {0};
    int fd = -1;

public:
    Spi();
    ~Spi();

public:
    virtual int init() override;
    virtual int exec(char *cmd) override;

public:
    static void *handler(void *handler);
};

#endif  // SPI
