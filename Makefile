include $(APPDIR)/Make.defs

PROGNAME = translator
APPNAME = translator
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(CONFIG_DEFAULT_TASK_STACKSIZE)
MODULE = $(CONFIG_TRANSLATOR)

MAINSRC = main.cpp
CXXSRCS += src/Translator.cpp
CXXSRCS += src/Interfaces/Base.cpp
CXXSRCS += src/Interfaces/Uart.cpp
CXXSRCS += src/Interfaces/Spi.cpp
CXXSRCS += src/Interfaces/I2c.cpp

CFLAGS += -DHAVE_CONFIG_H
CXXFLAGS += ${CFLAGS}
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/Interfaces
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/translator/src/RingBuffer

include $(APPDIR)/Application.mk
