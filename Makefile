include $(APPDIR)/Make.defs
MAINSRC = translator_main.cxx

PROGNAME = translator
APPNAME = translator
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(CONFIG_DEFAULT_TASK_STACKSIZE)
MODULE = $(CONFIG_TRANSLATOR)

include $(APPDIR)/Application.mk
