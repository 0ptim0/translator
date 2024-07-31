include $(APPDIR)/Make.defs

PROGNAME = translator
APPNAME = translator
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(CONFIG_DEFAULT_TASK_STACKSIZE)
MODULE = $(CONFIG_TRANSLATOR)

MAINSRC = main.cpp
# CSRCS += lib/libyaml/src/api.c
# CSRCS += lib/libyaml/src/dumper.c
# CSRCS += lib/libyaml/src/emitter.c
# CSRCS += lib/libyaml/src/loader.c
# CSRCS += lib/libyaml/src/parser.c
# CSRCS += lib/libyaml/src/reader.c
# CSRCS += lib/libyaml/src/scanner.c
# CSRCS += lib/libyaml/src/writer.c

CXXSRCS += src/Translator.cpp
CXXSRCS += src/Interfaces/Base.cpp
CXXSRCS += src/Interfaces/Uart.cpp
CXXSRCS += src/Interfaces/Spi.cpp
CXXSRCS += src/Interfaces/I2c.cpp

# CFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/lib/libyaml/include
# CFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/include
CFLAGS += -DHAVE_CONFIG_H
CXXFLAGS += ${CFLAGS}
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/Interfaces
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/RingBuffer

# VPATH += :lib/libyaml/src
# DEPPATH += --dep-path lib/libyaml/src

include $(APPDIR)/Application.mk
