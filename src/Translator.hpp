#ifndef TRANSLATOR
#define TRANSLATOR

#include "Interface.hpp"

constexpr int TRANSLATOR_CFG_MAX_PATH_LENGTH = 32;
constexpr char TRANSLATOR_CFG_DEFAULT_PATH[] = "/dev/fmc";

class Translator {
private:
    static Translator *inst;

#ifdef TRANSLATOR_SAVE_CONFIG
private:
    char path[TRANSLATOR_CFG_MAX_PATH_LENGTH] = {0};
    int fd = -1;
#endif

private:
    Interface *interfaces[IF_MAX_INTERFACES] = {0};

private:
    Translator();

public:
    Translator(const Translator &) = delete;
    void operator=(const Translator &) = delete;

public:
    static Translator *getInstance();

public:
    int daemon(int argc, char **argv);
    void daemon_help();

public:
    int commander(int argc, char **argv);
    void commander_help();

public:
    int routine();

private:
    int verbosity = 0;
};

#endif  // TRANSLATOR
