#
#  (c) 2005-2007 Laurent Vivier <Laurent@Vivier.EU>
#

TOP	= $(shell pwd)

68000FLAGS = -m68000 -Wa,-m68000
CFLAGS	+= -nostdlib -nodefaultlibs -Wall -Werror -Wno-multichar -fpic -O2 -Os
CPPFLAGS	= -I$(TOP) -DARCH_M68K

LIBRARY = libmacos.a

SOURCES = CloseDriver.c OpenDriver.c SerGetBuf.c GetDCtlEntry.c

HEADERS = libmacos.h \
	  macos/devices.h macos/errors.h macos/events.h macos/files.h \
	  macos/gestalt.h macos/init.h macos/lowmem.h \
	  macos/memory.h macos/osutils.h macos/quickdraw.h macos/romdefs.h \
	  macos/scsi.h macos/serial.h macos/slotmanager.h macos/traps.h \
	  macos/types.h macos/video.h

all: $(LIBRARY)

include $(TOP)/../tools.mk
include $(TOP)/../Rules.mk
