#!/bin/bash

# Make the lib directory...
mkdir lib

# iotivity-constrained
rm -rf lib/iotivity
git clone https://gerrit.iotivity.org/gerrit/p/iotivity-constrained.git lib/iotivity
# TinyCBOR...
git clone https://github.com/01org/tinycbor.git lib/iotivity/deps/tinycbor

# mbedTLS...
rm -rf lib/mbedtls
git clone https://github.com/ARMmbed/mbedtls.git lib/mbedtls

# Manuvr...
#rm -rf lib/mbedtls
#git clone https://github.com/Manuvr/ManuvrOS.git lib/ManuvrOS
