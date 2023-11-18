#ifndef TRANSLATOR
#define TRANSLATOR

#include "Interface.hpp"

constexpr int TRANSLATOR_MAX_INTERFACES = 8;
constexpr int TRANSLATOR_CFG_MAX_PATH_LENGTH = 32;
constexpr char TRANSLATOR_CFG_DEFAULT_PATH[] = "/fs/cfg.yaml";

class Translator {
private:
    char path[TRANSLATOR_CFG_MAX_PATH_LENGTH] = {0};
    int fd = -1;

private:
    Interface *interfaces[TRANSLATOR_MAX_INTERFACES];

public:
    Translator();
    ~Translator();

public:
    int init();
};


#endif // TRANSLATOR
