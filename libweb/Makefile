
include ../makefile.rc
MOD_NAME=web
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/onu/apps/lynx/web

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork  -mthumb -mcpu=arm9 \
-D_KERNEL -D__ECOS \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/cmn/cgi/cgi_cmn.c \
$(MOD_SRC_HOME)/cmn/cgi/cgi_auth.c \
$(MOD_SRC_HOME)/cmn/cgi/cgi_system.c \
$(MOD_SRC_HOME)/cmn/cgi/cgi_upgrade.c \
$(MOD_SRC_HOME)/cs/cgi/cgi_cs_init.c \
$(MOD_SRC_HOME)/cs/cgi/cgi_cs_port_config.c \
$(MOD_SRC_HOME)/cs/cgi/cgi_cs_sys_config.c \
$(MOD_SRC_HOME)/cs/cgi/cgi_cs_sys_status.c \
$(MOD_SRC_HOME)/cs/cgi/cgi_cs_user_config.c 


MOD_INCLUDES= -I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/apps/lynx/pppoe  \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/aal/lynxd \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/plat/lynxd/mports/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/onu/apps/lynx/loopdetect \
-I$(SWHOME)/src/cmn/apps/oam/include/ \
-I$(SWHOME)/src/onu/sdl/cmn/cmn/ \
-I$(SWHOME)/src/cmn/apps/mcast/inc/ \
-I$(SWHOME)/src/onu/apps/lynx/stp/inc/ \
-I$(SWHOME)/src/onu/apps/lynx/ipintf \
-I$(MOD_SRC_HOME)/cmn/cgi/include/ \
-I$(MOD_SRC_HOME)/cs/cgi/include/ \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn

include $(SWHOME)/tools/support/build/makefile.mod_start

