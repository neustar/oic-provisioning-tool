#!/bin/bash

# Make the lib directory...
mkdir lib

# JSON support via jansson...
# Note that we do special-handling here to make the build-process smoother...
rm -rf lib/jansson
git clone https://github.com/akheron/jansson.git lib/jansson
ln -s `pwd`/lib/jansson/src/ lib/jansson/include

# MQTT, if desired.
# Note that we do special-handling here to make the build-process smoother...
rm -rf lib/paho.mqtt.embedded-c
git clone https://github.com/eclipse/paho.mqtt.embedded-c.git lib/paho.mqtt.embedded-c
cp lib/paho.mqtt.embedded-c/MQTTPacket/src/* lib/paho.mqtt.embedded-c/

# CBOR...
# Note that we do special-handling here to make the build-process smoother...
rm -rf lib/cbor-cpp
git clone https://github.com/naphaso/cbor-cpp.git lib/cbor-cpp
ln -s `pwd`/lib/cbor-cpp/src/ lib/cbor-cpp/include

# Avro. Again, we will shuffle things a bit to make inclusion uniform.
# Note that Avro requires jansson for JSON support.
rm -rf lib/avro
git clone https://github.com/apache/avro.git lib/avro
ln -s `pwd`/lib/avro/lang/c/src/ lib/avro/include

# mbedTLS...
rm -rf lib/mbedtls
git clone https://github.com/ARMmbed/mbedtls.git lib/mbedtls

# iotivity-constrained
rm -rf lib/iotivity
git clone https://gerrit.iotivity.org/gerrit/p/iotivity-constrained.git lib/iotivity

# Return...
cd ..
