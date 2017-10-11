
include ../makefile.rc
MOD_NAME=trmsvr
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 -include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/onu/apps/lynx/trmsvr/terminal_server.c \
$(SWHOME)/src/onu/apps/lynx/trmsvr/terminal_server_adapt.c \
$(SWHOME)/src/onu/apps/lynx/trmsvr/terminal_server_api.c 

MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/peripheral \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/onu/apps/lynx/trmsvr \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn	\
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc	\
-I$(SWHOME)/src/cmn/apps/oam/include	\
-I$(SWHOME)/src/onu/apps/gwd


include $(SWHOME)/tools/support/build/makefile.mod_start

