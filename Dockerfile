# TODO:
# https://bugs.alpinelinux.org/issues/1579
#FROM frolvlad/alpine-glibc
## TODO: Cutting bash will save 5 dependencies. But RUN ./downloadDeps.sh breaks.
#RUN apk add --no-cache --update make gcc g++ git bash

FROM ubuntu

MAINTAINER Name <ian.lindsay@neustar.biz>

RUN apt-get update && apt-get install -y build-essential git

RUN mkdir -p /root/
WORKDIR /root/

COPY lib lib
COPY src src
COPY downloadDeps.sh downloadDeps.sh
COPY Makefile Makefile

RUN chmod +x /root/downloadDeps.sh && /root/downloadDeps.sh

# TCP 5686: Program console (Use netcat)
EXPOSE 5686
