include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

include $(DEVKITARM)/3ds_rules

CROSS_COMPILE = arm-none-eabi-
BUILD_VERBOSE=1
V=1
NO_SMDH = 1
LD = $(CC)

INC += -I.
INC += -I..
INC += -I$(BUILD)
INC += -I$(DEVKITARM)/include
INC += -I$(CTRULIB)/include
INC += -I$(DEVKITPRO)/libsf2d/include

CFLAGS_ARCH = -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS = -g -Wall -mword-relocations -fomit-frame-pointer -ffunction-sections $(CFLAGS_ARCH) $(INC)
CFLAGS += -DARM11 -D_3DS

LDFLAGS = -specs=3dsx.specs -g $(CFLAGS_ARCH) -Wl,-Map,$(notdir $*.map) -L$(CTRULIB)/lib -L$(DEVKITPRO)/libsf2d/lib
LIBS = -lsf2d -lctru -lm 

SRC_C = $(shell find -type f -name '*.c')

SRC_S = \
#	startup_stm32f40xx.s \
	gchelper.s \

OFILES = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))
OBJ = $(OFILES)

DATA = data

all: $(BUILD)/python.3dsx

$(OFILES): | $(HEADER_BUILD)/qstrdefs.generated.h $(HEADER_BUILD)/mpversion.h

ifeq ($(strip $(NO_SMDH)),)
$(BUILD)/python.3dsx: $(BUILD)/python.elf $(BUILD)/python.smdh
else
$(BUILD)/python.3dsx: $(BUILD)/python.elf
endif

$(BUILD)/python.elf: $(OFILES)

include ../py/mkrules.mk
