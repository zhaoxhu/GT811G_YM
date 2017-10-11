
include ../makefile.rc
MOD_NAME=ipintf
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/onu/apps/lynx/ipintf

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Wno-uninitialized -Werror -nostdlib -fno-inline \
-D_KERNEL -D__ECOS -D_CYGPKG_LWIP_STACK_\
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/app_ip_cmd.c \
$(MOD_SRC_HOME)/app_ip_util.c \
$(MOD_SRC_HOME)/app_ip_api.c \
$(MOD_SRC_HOME)/app_ip_init.c \
$(MOD_SRC_HOME)/app_ip_arp.c \
$(MOD_SRC_HOME)/app_ip_mgnt.c \
$(MOD_SRC_HOME)/app_ip_adapter.c \
$(MOD_SRC_HOME)/app_ip_intf.c 

#$(MOD_SRC_HOME)/app_ip_intf.c \
$(MOD_SRC_HOME)/app_ip_arp.c \
$(MOD_SRC_HOME)/app_ip_init.c \
$(MOD_SRC_HOME)/app_ip_api.c \
$(MOD_SRC_HOME)/app_ip_cmd.c \
$(MOD_SRC_HOME)/app_ip_mgnt.c 


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/cmn/apps/oam/include \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/ipintf \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/cmn/apps/core \
-I$(SWHOME)/src/cmn/apps/event \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/cmn/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/sc/imst/include \
-I$(SWHOME)/src/onu/cmd \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn	\
-I$(SWHOME)/src/onu/apps/gwd

include $(SWHOME)/tools/support/build/makefile.mod_start

