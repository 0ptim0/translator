PROBE_INC += -I../tools/probe/
PROBE_SRC += ../tools/probe/probe.c
DEFINES += -DPROBE_USE_STDIO
CXX = g++

CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -Wpedantic
CFLAGS += -Wshadow
CFLAGS += -Wformat=2
CFLAGS += -Wfloat-equal
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-enum
CFLAGS += -Winit-self
CFLAGS += -Wmissing-include-dirs
CFLAGS += -Wstrict-aliasing=2
CFLAGS += -Wunreachable-code
CFLAGS += -Wundef
CFLAGS += -Wredundant-decls
CFLAGS += -Wlogical-op
CFLAGS += -Waggregate-return
CFLAGS += -Wredundant-move
CFLAGS += -Wsuggest-final-methods
CFLAGS += -Wsuggest-final-types
CFLAGS += -Wsuggest-override

BASE_IF_SRC += ../src/Interfaces/Base.cpp
BASE_IF_SRC += TestInterface.cpp
BASE_IF_INC += -I../src/Interfaces/
BASE_IF_INC += -I../src/RingBuffer/

test_interface:
	$(CXX) $(PROBE_INC) $(PROBE_SRC) $(BASE_IF_SRC) $(BASE_IF_INC) $(DEFINES) $(CFLAGS) -o TestInterface
