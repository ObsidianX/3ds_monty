include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

include $(DEVKITARM)/3ds_rules
include cia_rules

APP_TITLE = Monty3DS
APP_TITLE_LONG = Monty3DS NetLoader
APP_DESCRIPTION = Python for the 3DS
APP_AUTHOR = ObsidianX
APP_ICON = python.png
APP_PRODUCT_CODE = CTR-P-MTYP
APP_UNIQUE_ID = 0x88001

BANNER_IMAGE = banner.png
BANNER_AUDIO = python.wav

CROSS_COMPILE = arm-none-eabi-
BUILD_VERBOSE=1
V=1
LD = $(CC)
export _3DSXFLAGS += --smdh=$(BUILD)/monty.smdh --romfs=$(CURDIR)/romfs

INC += -I.
INC += -I..
INC += -I$(BUILD)
INC += -I$(DEVKITARM)/include
INC += -I$(CTRULIB)/include
INC += -I$(DEVKITPRO)/libsf2d/include
INC += -I$(DEVKITPRO)/portlibs/armv6k/include

L_INC = -L$(CTRULIB)/lib
L_INC += -L$(DEVKITPRO)/libsf2d/lib
L_INC += -L$(DEVKITPRO)/portlibs/armv6k/lib

CFLAGS_ARCH = -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS = -ggdb -O2 -Wall -mword-relocations -fomit-frame-pointer -ffunction-sections $(CFLAGS_ARCH) $(INC) -Os
CFLAGS += -DARM11 -D_3DS -D_3DS_MODE=$(_3DS_MODE)

LDFLAGS = -specs=3dsx.specs -g $(CFLAGS_ARCH) -Wl,-Map,$(notdir $*.map) $(L_INC)
LIBS = -lsftd -lsf2d -lzipfs -lctru -lfreetype -lpng -ljpeg -lz -lm

SRC_C = $(shell find -type f -name '*.c')

SRC_S = \
#	startup_stm32f40xx.s \
	gchelper.s \

OFILES = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))
OBJ = $(OFILES)

DATA = data


$(OFILES): | $(HEADER_BUILD)/qstrdefs.generated.h $(HEADER_BUILD)/mpversion.h

$(BUILD)/monty.smdh:

ifeq ($(strip $(_3DS_MODE)),)
_3DS_MODE = 1
endif

ifeq ($(_3DS_MODE), 1)
all: $(BUILD)/monty_netload.cia $(BUILD)/monty_netload.3dsx

$(BUILD)/monty_netload.3dsx: $(BUILD)/monty_netload.elf $(BUILD)/monty.smdh
$(BUILD)/monty_netload.elf: $(OFILES) 
$(BUILD)/monty_netload.cia: $(BUILD)/monty_netload.elf $(BUILD)/banner_netload.bnr $(BUILD)/icon_netload.icn
endif
ifeq ($(_3DS_MODE), 2)
all: $(BUILD)/monty_romfs.elf

$(BUILD)/monty_romfs.elf: $(OFILES)
endif
ifeq ($(_3DS_MODE), 3)
all: $(BUILD)/monty_argv.3dsx

$(BUILD)/monty_argv.elf: $(OFILES)
$(BUILD)/monty_argv.3dsx: $(BUILD)/monty_argv.elf $(BUILD)/monty.smdh
endif

include ../py/mkrules.mk
