#!/bin/bash

# iotivity-constrained
rm -rf lib/iotivity
git clone https://github.com/iotivity/iotivity-constrained.git lib/iotivity

#
# HACK: Pin to this version of iotivity-contrained  because changes are breaking the build.
#       (Its a good idea to pin all dependency versions anyways :P)
#
cd lib/iotivity
git reset --hard 96b50da1762d016790ac7fa516d95a0252bb2f72
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
