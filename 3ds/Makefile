include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

include $(DEVKITARM)/3ds_rules
include cia_rules

APP_TITLE = Monty3DS
APP_DESCRIPTION = Python for the 3DS
APP_AUTHOR = ObsidianX
APP_ICON = python.png
APP_PRODUCT_CODE = CTR-P-MTYP
APP_UNIQUE_ID = 0x88001

BANNER_IMAGE = banner.png
BANNER_AUDIO = ni.wav

CROSS_COMPILE = arm-none-eabi-
BUILD_VERBOSE=1
V=1
LD = $(CC)
export _3DSXFLAGS += --smdh=$(BUILD)/python.smdh --romfs=$(CURDIR)/romfs

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
CFLAGS = -g -Wall -mword-relocations -fomit-frame-pointer -ffunction-sections $(CFLAGS_ARCH) $(INC) -Os
CFLAGS += -DARM11 -D_3DS

LDFLAGS = -specs=3dsx.specs -g $(CFLAGS_ARCH) -Wl,-Map,$(notdir $*.map) $(L_INC)
LIBS = -lsftd -lsf2d -lctru -lfreetype -lpng -ljpeg -lz -lm

SRC_C = $(shell find -type f -name '*.c')

SRC_S = \
#	startup_stm32f40xx.s \
	gchelper.s \

OFILES = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))
OBJ = $(OFILES)

DATA = data

all: $(BUILD)/python.3dsx
#$(BUILD)/python.cia


$(OFILES): | $(HEADER_BUILD)/qstrdefs.generated.h $(HEADER_BUILD)/mpversion.h

$(BUILD)/python.smdh:

ifeq ($(strip $(NO_SMDH)),)
$(BUILD)/python.3dsx: $(BUILD)/python.elf $(BUILD)/python.smdh
else
$(BUILD)/python.3dsx: $(BUILD)/python.elf
endif

$(BUILD)/python.cia: $(BUILD)/python.elf $(BUILD)/banner.bnr $(BUILD)/icon.icn

$(BUILD)/python.elf: $(OFILES)

include ../py/mkrules.mk
