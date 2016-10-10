#!/bin/bash

# iotivity-constrained
rm -rf lib/iotivity
git clone https://github.com/iotivity/iotivity-constrained.git lib/iotivity

#
# HACK: Pin to this version of iotivity-contrained  because changes are breaking the build.
#       (Its a good idea to pin all dependency versions anyways :P)
#
cd lib/iotivity
git reset --hard a79077f9646edc0814b5800ef49be9d73178a220
cd ../..


# TinyCBOR...
git clone https://github.com/01org/tinycbor.git lib/iotivity/deps/tinycbor

# mbedTLS...
rm -rf lib/mbedtls
git clone https://github.com/ARMmbed/mbedtls.git lib/mbedtls

# Manuvr...
rm -rf lib/ManuvrOS
git clone https://github.com/Manuvr/ManuvrOS.git lib/ManuvrOS
cd lib/ManuvrOS
cd ../..
