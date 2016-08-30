###########################################################################
# Makefile for Neustar OIC provisioning tools.
# Author: J. Ian Lindsay
# Date:   2016.08.30
#
###########################################################################

OPTIMIZATION       = -O2
C_STANDARD         = gnu99
CPP_STANDARD       = gnu++11
FIRMWARE_NAME      = neudev
CFLAGS             = -Wall
NEUDEV_OPTIONS     = -D_GNU_SOURCE

###########################################################################
# Environmental awareness...
###########################################################################
SHELL          = /bin/sh
WHERE_I_AM     = $(shell pwd)

export CXX     = $(shell which g++)
export CC      = $(shell which gcc)
export SZ      = $(shell which size)
export MAKE    = $(shell which make)

export OUTPUT_PATH = $(WHERE_I_AM)/build/


###########################################################################
# Source files, includes, and linker directives...
###########################################################################
INCLUDES    = -I$(WHERE_I_AM)/.
INCLUDES   += -I$(WHERE_I_AM)/src
INCLUDES   += -I$(WHERE_I_AM)/lib
INCLUDES   += -I$(WHERE_I_AM)/lib/paho.mqtt.embedded-c/
INCLUDES   += -I$(WHERE_I_AM)/lib/mbedtls/include/

# Libraries to link
LIBS = -L$(OUTPUT_PATH) -L$(WHERE_I_AM)/lib -lstdc++ -lm

# Wrap the include paths into the flags...
CFLAGS += $() $(INCLUDES)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion


###########################################################################
# Are we on a 64-bit system? If so, we'll need to specify
#   that we want a 32-bit build...
#
# Thanks, estabroo...
# http://www.linuxquestions.org/questions/programming-9/how-can-make-makefile-detect-64-bit-os-679513/
###########################################################################
LBITS = $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
	# This is no longer required on 64-bit platforms. But it is being retained in
	#   case 32-bit problems need to be debugged.
  #CFLAGS += -m32
endif


###########################################################################
# Source file and options definitions...
# TODO: I badly need to learn to write autoconf scripts....
#   I've at least tried to modularize to make the invariable transition less-painful.
###########################################################################
# This project has a single source file.
CPP_SRCS  = main.cpp

# mbedTLS will require this in order to use our chosen options.
export MBEDTLS_CONFIG_FILE = $(WHERE_I_AM)/mbedTLS_conf.h

# Since we are building on linux, we will have threading support via
# pthreads.
LIBS += -lpthread $(OUTPUT_PATH)/extraprotocols.a
LIBS += $(OUTPUT_PATH)/libmbedtls.a
LIBS += $(OUTPUT_PATH)/libmbedx509.a
LIBS += $(OUTPUT_PATH)/libmbedcrypto.a


###########################################################################
# Rules for building the program follow...
###########################################################################
# Merge our choices and export them to the downstream Makefiles...
CFLAGS += $(NEUDEV_OPTIONS)
export CFLAGS
export CPP_FLAGS    = $(CFLAGS) -fno-rtti -fno-exceptions


.PHONY: all


all: clean libs
	@echo '======================================================'
	$(MAKE) -C src/
	$(CXX) -static -o $(FIRMWARE_NAME) $(CPP_SRCS) $(CFLAGS) -std=$(CPP_STANDARD) $(LIBS) $(OPTIMIZATION)
	$(SZ) $(FIRMWARE_NAME)

debug: clean libs
	$(MAKE) debug -C src/
	$(CXX) -static -g -o $(FIRMWARE_NAME) $(CPP_SRCS) $(CFLAGS) -std=$(CPP_STANDARD) $(LIBS) -O0
	$(SZ) $(FIRMWARE_NAME)

tests: libs
	$(CXX) -static -o tests tests/tests.cpp $(CPP_FLAGS) -std=$(CPP_STANDARD) $(LIBS) $(OPTIMIZATION)
	$(CXX) -static -g -o tests-unoptimized tests/tests.cpp $(CPP_FLAGS) -std=$(CPP_STANDARD) $(LIBS) -O0

examples: libs
	$(CXX) -static -o lost-puppy examples/lost-puppy.cpp $(CPP_FLAGS) -std=$(CPP_STANDARD) $(LIBS) $(OPTIMIZATION)

builddir:
	mkdir -p $(OUTPUT_PATH)

libs: builddir
	$(MAKE) -C lib/

clean:
	$(MAKE) clean -C src/
	rm -f *.o *.su *~ lost-puppy $(FIRMWARE_NAME)
	rm -rf $(OUTPUT_PATH)

fullclean: clean
	$(MAKE) clean -C lib/
	rm -rf doc/doxygen

docs:
	doxygen Doxyfile

stats:
	find ./src -type f \( -name \*.cpp -o -name \*.h \) -exec wc -l {} +
