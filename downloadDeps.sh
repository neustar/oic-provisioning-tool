#!/bin/bash

# iotivity-constrained
rm -rf lib/iotivity
git clone https://gerrit.iotivity.org/gerrit/p/iotivity-constrained.git lib/iotivity
# TinyCBOR...
git clone https://github.com/01org/tinycbor.git lib/iotivity/deps/tinycbor

# mbedTLS...
rm -rf lib/mbedtls
git clone https://github.com/ARMmbed/mbedtls.git lib/mbedtls

# Manuvr...
#rm -rf lib/ManuvrOS
git clone https://github.com/jspark311/ManuvrOS.git lib/ManuvrOS
