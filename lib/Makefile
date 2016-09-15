###########################################################################
# Makefile for libraries
# Author: J. Ian Lindsay
###########################################################################

# Build all libraries
C_STANDARD   = gnu99
CPP_STANDARD = gnu++11

SOURCES_C      =
SOURCES_CPP    =
LIBS_TO_COPY   = mbedtls/library/*.a

INCLUDES        = -iquote.
LOCAL_C_FLAGS   = -nostdlib $(CFLAGS)
LOCAL_CXX_FLAGS = -nostdlib $(CPP_FLAGS)


###########################################################################
# iotivity-constrained.
# TODO: Much redundant. Refactor to use our native CBOR and MBED.
DTLS_SRC      = iotivity/deps/tinydtls/ccm.c
DTLS_SRC     += iotivity/deps/tinydtls/hmac.c
DTLS_SRC     += iotivity/deps/tinydtls/netq.c
DTLS_SRC     += iotivity/deps/tinydtls/peer.c
DTLS_SRC     += iotivity/deps/tinydtls/dtls_time.c
DTLS_SRC     += iotivity/deps/tinydtls/session.c
DTLS_SRC     += iotivity/deps/tinydtls/sha2/sha2.c
DTLS_SRC     += iotivity/deps/tinydtls/ecc/ecc.c
DTLS_SRC     += iotivity/deps/tinydtls/aes/rijndael.c
DTLS_SRC     += iotivity/deps/tinydtls/crypto.c
DTLS_SRC     += iotivity/deps/tinydtls/dtls.c

CBOR_SRC      = iotivity/deps/tinycbor/src/cborencoder.c
CBOR_SRC     += iotivity/deps/tinycbor/src/cborencoder_close_container_checked.c
CBOR_SRC     += iotivity/deps/tinycbor/src/cborparser.c

DTLSFLAGS = -DDTLSV12 -DWITH_SHA256 -DDTLS_CHECK_CONTENTTYPE -DWITH_OCF -I../../deps/tinydtls -DNDEBUG

IOTIVITY_SRC  = $(wildcard iotivity/util/*.c) ${CBOR_SRC}
IOTIVITY_SRC += $(wildcard iotivity/messaging/coap/*.c iotivity/api/*.c)
IOTIVITY_SRC += iotivity/port/linux/ipadapter.c
IOTIVITY_SRC += iotivity/port/linux/storage.c
VPATH = iotivity/messaging/coap/:iotivity/util/:iotivity/api/:iotivity/deps/tinycbor/src/:iotivity/include/

###########################################################################
# Option conditionals
###########################################################################
ifeq ($(SECURE),1)
	LOCAL_C_FLAGS += -DOC_SECURITY ${DTLSFLAGS}
	VPATH += ../../security/:../../deps/tinydtls/:../../deps/tinydtls/aes/:../../deps/tinydtls/sha2/:../../deps/tinydtls/ecc/:
	IOTIVITY_SRC += $(DTLS_SRC) $(wildcard iotivity/security/*.c)
	INCLUDES   += -I./iotivity/deps/tinydtls
endif

ifeq ($(DEBUG),1)
LOCAL_C_FLAGS += -DDEBUG
endif

###########################################################################
# Parameter unification and make targets
###########################################################################
SOURCES_C  += $(IOTIVITY_SRC)

OBJS  = $(SOURCES_C:.c=.o) $(SOURCES_CPP:.cpp=.o)
LIBNAME = libextras.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(INCLUDES) $(LOCAL_C_FLAGS) -std=$(C_STANDARD) -c $< -o $@

%.o : %.cpp
	$(CXX) $(INCLUDES) $(LOCAL_CXX_FLAGS) -std=$(CPP_STANDARD) -c $< -o $@

$(LIBNAME): externals $(OBJS)
	$(AR) -rvs $(OUTPUT_PATH)/$@ $(OBJS)
	cp -RP $(LIBS_TO_COPY) $(OUTPUT_PATH)

externals:
	$(MAKE) lib -C mbedtls/
	$(MAKE) -C ManuvrOS/ManuvrOS/

clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT_PATH)/$(LIBNAME)
	$(MAKE) clean -C mbedtls/
	$(MAKE) clean -C ManuvrOS/ManuvrOS/
