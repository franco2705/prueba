# Minimal Nintendo Switch homebrew build configuration.

TARGET      := cube3d
BUILD       := build
SOURCES     := source
INCLUDES    := include

APP_TITLE   := Cube 3D Base
APP_AUTHOR  := Homebrew starter
APP_VERSION := 0.1.0

ARCH        := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -ftls-model=local-exec
CFLAGS      := -g -Wall -Wextra -O2 -ffunction-sections -fdata-sections $(ARCH)
CXXFLAGS    := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS     := -g $(ARCH)
LDFLAGS     := -g $(ARCH) -Wl,--gc-sections
LIBS        := -lnx -lm

include $(DEVKITPRO)/libnx/switch_rules
