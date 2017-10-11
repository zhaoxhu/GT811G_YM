
include ../makefile.rc
MOD_NAME=db
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn/db_mngt.c \
$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc/db_instance.c 


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/cmn/apps/mcast/inc \
-I$(SWHOME)/src/onu/apps/lynx/mcast \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/plat/lynxd \
-I$(SWHOME)/src/onu/plat/lynxd/mports/ctc \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/pppoe \
-I$(SWHOME)/src/onu/apps/lynx/ipintf \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/onu/apps/lynx/loopdetect \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/cmn/apps/oam/include \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/aal/lynxd/ \
-I$(SWHOME)/src/onu/apps/lynx/stp/inc \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn

include $(SWHOME)/tools/support/build/makefile.mod_start

