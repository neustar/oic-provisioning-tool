###########################################################################
# Makefile for Neustar OIC provisioning tools.
# Author: J. Ian Lindsay
# Date:   2016.08.30
###########################################################################
OPTIMIZATION       = -O1
C_STANDARD         = gnu99
CPP_STANDARD       = gnu++11
CFLAGS             = -Wall
NEUDEV_OPTIONS     = -D_GNU_SOURCE


###########################################################################
# Environmental awareness...
###########################################################################
SHELL          = /bin/sh
WHERE_I_AM     = $(shell pwd)

export CC      = $(shell which gcc)
export CXX     = $(shell which g++)
export AR      = $(shell which ar)
export SZ      = $(shell which size)
export MAKE    = $(shell which make)

export OUTPUT_PATH = $(WHERE_I_AM)/build/


###########################################################################
# Source files, includes, and linker directives...
###########################################################################
INCLUDES    = -I$(WHERE_I_AM)/.
INCLUDES   += -I$(WHERE_I_AM)/lib/ManuvrOS/ManuvrOS
INCLUDES   += -I$(WHERE_I_AM)/lib
INCLUDES   += -I$(WHERE_I_AM)/lib/mbedtls/include
INCLUDES   += -I$(WHERE_I_AM)/lib/iotivity
INCLUDES   += -I$(WHERE_I_AM)/lib/iotivity/include

# Libraries to link
LIBS = -L$(OUTPUT_PATH) -L$(WHERE_I_AM)/lib -lstdc++ -lm

# Wrap the include paths into the flags...
CFLAGS += $(INCLUDES)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion


###########################################################################
# Source file and options definitions...
# TODO: I badly need to learn to write autoconf scripts....
#   I've at least tried to modularize to make the invariable transition less-painful.
###########################################################################
# This project has a single source file.
CPP_SRCS  =

NEUDEV_OPTIONS += -D__MANUVR_LINUX
NEUDEV_OPTIONS += -DMANUVR_CONSOLE_SUPPORT
NEUDEV_OPTIONS += -DMANUVR_STORAGE
NEUDEV_OPTIONS += -DMANUVR_CBOR
NEUDEV_OPTIONS += -DMANUVR_SUPPORT_TCPSOCKET
NEUDEV_OPTIONS += -DMANUVR_OPENINTERCONNECT
NEUDEV_OPTIONS += -DWITH_MBEDTLS
NEUDEV_OPTIONS += -DOC_SECURITY

# Since we are building on linux, we will have threading support via
# pthreads.
LIBS += -lmanuvr -lextras -lpthread
LIBS += $(OUTPUT_PATH)/libmbedtls.a
LIBS += $(OUTPUT_PATH)/libmbedx509.a
LIBS += $(OUTPUT_PATH)/libmbedcrypto.a

# Support for specific SBC hardware on linux.
ifeq ($(BOARD),RASPI)
NEUDEV_OPTIONS += -DRASPI
export MANUVR_BOARD = RASPI
endif

# Debugging options...
ifeq ($(DEBUG),1)
NEUDEV_OPTIONS += -D__MANUVR_DEBUG
NEUDEV_OPTIONS += -D__MANUVR_PIPE_DEBUG
NEUDEV_OPTIONS += -D__MANUVR_EVENT_PROFILER
OPTIMIZATION    = -O0 -g
export DEBUG=1
endif

# Framework selections, if any are desired.
ifeq ($(OIC_SERVER),1)
	NEUDEV_OPTIONS += -DOC_SERVER
	CPP_SRCS += src/lost-puppy.cpp
	CPP_SRCS += src/LostPuppy/LostPuppy.cpp
else ifeq ($(OIC_CLIENT),1)
	NEUDEV_OPTIONS += -DOC_CLIENT
	CPP_SRCS += src/main.cpp
	CPP_SRCS += src/Provisioner/Provisioner.cpp
endif

# mbedTLS will require this in order to use our chosen options.
NEUDEV_OPTIONS += -DMBEDTLS_CONFIG_FILE='<mbedTLS_conf.h>'


###########################################################################
# Rules for building the program follow...
###########################################################################
# Merge our choices and export them to the downstream Makefiles...
CFLAGS += $(NEUDEV_OPTIONS) $(OPTIMIZATION)

export CFLAGS
export CPP_FLAGS    = $(CFLAGS) -fno-rtti -fno-exceptions

# Tweak the environment for iotivity-constrained.
export MANUVR_PLATFORM=LINUX
export SECURE=1

#.PHONY: all
# TODO: "make" should build both tools.


neudev: libs
	$(CXX) -static -o neudev $(CPP_SRCS) $(CPP_FLAGS) -std=$(CPP_STANDARD) $(LIBS)
	$(SZ) neudev

lostpuppy: libs
	$(CXX) -static -o lostpuppy $(CPP_SRCS) $(CPP_FLAGS) -std=$(CPP_STANDARD) $(LIBS)
	$(SZ) lostpuppy

builddir:
	mkdir -p $(OUTPUT_PATH)

libs: builddir
	$(MAKE) -C lib/

clean:
	rm -f *.o *.su *~ lostpuppy neudev

fullclean: clean
	rm -rf $(OUTPUT_PATH)
	$(MAKE) clean -C lib/
