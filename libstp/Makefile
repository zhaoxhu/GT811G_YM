
include ../makefile.rc
MOD_NAME=stp
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(SWHOME)/src/onu/apps/lynx/stp/src/edge.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/migrate.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/p2p.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/pcost.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/port.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/portinfo.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/rolesel.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/roletrns.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/statmch.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stpm.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_to.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/sttrans.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/times.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/topoch.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/transmit.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/vector.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stpmgmt.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_in.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_api.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_init.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_test.c \
$(SWHOME)/src/onu/apps/lynx/stp/src/stp_sdl_adapt.c \


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/onu/apps/lynx/stp/inc \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/aal/lynxd/ \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn 

include $(SWHOME)/tools/support/build/makefile.mod_start

