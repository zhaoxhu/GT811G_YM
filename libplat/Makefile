
include ../makefile.rc
MOD_NAME=plat
MOD_TARGET=library
MOD_SRC_HOME=$(SWHOME)/src/cmn/plat

ECOS_INSTALL_HOME=../iros/install
IROS_CONFIG_H=$(shell pwd)/../iros_config.h

MOD_CFLAGS= -Werror -Wno-uninitialized -nostdlib -fno-inline \
-mthumb-interwork -mthumb -mcpu=arm9 \
-include $(IROS_CONFIG_H)

CFILES= \
$(MOD_SRC_HOME)/util/util_list.c \
$(MOD_SRC_HOME)/util/util_hash.c \
$(MOD_SRC_HOME)/peripheral/ssp.c \
$(MOD_SRC_HOME)/peripheral/i2c.c \
$(MOD_SRC_HOME)/peripheral/mdio.c \
$(MOD_SRC_HOME)/peripheral/gpio.c \
$(MOD_SRC_HOME)/peripheral/watchdog.c \
$(MOD_SRC_HOME)/peripheral/uart.c \
$(MOD_SRC_HOME)/sdl/sdl_ssp.c \
$(MOD_SRC_HOME)/sdl/sdl_i2c.c \
$(MOD_SRC_HOME)/sdl/sdl_mdio.c \
$(MOD_SRC_HOME)/sdl/sdl_gpio.c \
$(MOD_SRC_HOME)/sdl/sdl_watchdog.c \
$(MOD_SRC_HOME)/sdl/sdl_uart.c \
$(MOD_SRC_HOME)/image.c \
$(MOD_SRC_HOME)/glb_reg_api.c \
$(MOD_SRC_HOME)/shell.c \
$(MOD_SRC_HOME)/plat_cli.c \
$(MOD_SRC_HOME)/cpu_load.c \
$(MOD_SRC_HOME)/tlv_parser.c \
$(MOD_SRC_HOME)/xyzmodem.c \
$(MOD_SRC_HOME)/startup_cfg.c \
$(MOD_SRC_HOME)/io.c \
$(MOD_SRC_HOME)/iros_cmn.c \
$(MOD_SRC_HOME)/post.c \
$(MOD_SRC_HOME)/log.c \
$(MOD_SRC_HOME)/exception.c \
$(MOD_SRC_HOME)/interrupt.c \
$(MOD_SRC_HOME)/cs_timer.c \
$(MOD_SRC_HOME)/cs_module.c \
$(MOD_SRC_HOME)/mempool.c \
$(MOD_SRC_HOME)/boot_seq.c \
$(SWHOME)/src/onu/plat/lynxd/dma.c \
$(SWHOME)/src/onu/cmd/plat_cmd.c \
$(MOD_SRC_HOME)/cli/cli_common.c \
$(MOD_SRC_HOME)/cli/cli_parser.c \
$(MOD_SRC_HOME)/cli/telnetd.c \
$(MOD_SRC_HOME)/cli/cli_cmd_demo.c \
$(MOD_SRC_HOME)/cli/cli_user_register.c \
$(MOD_SRC_HOME)/polling.c \
$(SWHOME)/src/onu/apps/lynx/gpio/cmn/app_gpio.c \
$(SWHOME)/src/onu/apps/lynx/gpio/ctc/app_gpio_ctc_mports.c \
$(SWHOME)/src/onu/apps/gwd/gwd_cli.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_qinq.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_mc_process.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_dhcp.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_broadstorm.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_log.c	\
$(SWHOME)/src/onu/apps/gwd/gwd_utils.c	\


MOD_INCLUDES=-I$(ECOS_INSTALL_HOME)/include \
-I$(SWHOME)/src/cmn/plat/include \
-I$(SWHOME)/src/cmn/plat/osal/ecos \
-I$(SWHOME)/src/cmn/plat/sdl \
-I$(SWHOME)/src/include \
-I$(SWHOME)/src/onu/aal/lynxd/include \
-I$(SWHOME)/src/onu/sdl/include/cmn \
-I$(SWHOME)/src/onu/sdl/include \
-I$(SWHOME)/src/onu/apps/lynx/cortina \
-I$(SWHOME)/src/onu/plat/lynxd \
-I$(SWHOME)/src/onu/plat/lynxd/mports/ctc \
-I$(SWHOME)/src/cmn/apps/oam/include \
-I$(SWHOME)/src/onu/cmd \
-I$(SWHOME)/src/cmn/apps/event \
-I$(SWHOME)/src/onu/sdl/include/ctc \
-I$(SWHOME)/src/onu/aal/lynxd/ \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/ctc \
-I$(SWHOME)/src/onu/apps/lynx/dbmngt/cmn \
-I$(SWHOME)/src/onu/apps/lynx/ipintf \
-I$(SWHOME)/src/onu/apps/lynx/stp/inc \
-I$(SWHOME)/src/cmn/apps/mcast/inc \
-I$(SWHOME)/src/onu/apps/lynx/mcast \
-I$(SWHOME)/src/onu/apps/lynx/oam/ctc \
-I$(SWHOME)/src/onu/apps/lynx/pppoe \
-I$(SWHOME)/src/onu/apps/lynx/gpio/cmn	\
-I$(SWHOME)/src/onu/apps/gwd/	\
-I$(SWHOME)/src/onu/sdl/cmn/cmn/	\
-I$(SWHOME)/ecos/ecos-2.1/packages/net/lwip_tcpip/current/include/lwip	\
-I$(SWHOME)/src/onu/3rd/driver/realtek	\
-I$(SWHOME)/src/onu/apps/lynx/trmsvr \


include $(SWHOME)/tools/support/build/makefile.mod_start
