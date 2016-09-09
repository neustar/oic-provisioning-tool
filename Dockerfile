FROM frolvlad/alpine-glibc

# TODO: Cutting bash will save 5 dependencies. But RUN ./downloadDeps.sh breaks.
RUN apk add --no-cache --update make g++ git bash

RUN mkdir -p /root/
WORKDIR /root/

COPY lib lib
COPY src src
COPY downloadDeps.sh downloadDeps.sh
COPY Makefile Makefile

RUN chmod +x /root/downloadDeps.sh && /root/downloadDeps.sh

ENV SECURE=1 DEBUG=1 OIC_SERVER=1

# TODO: Would rather use ENV...
RUN make lostpuppy SECURE=1 DEBUG=1 OIC_SERVER=1

# UDP 5683: coap
# UDP 5684: coaps
# TCP 5685: Onboarding results
# TCP 5686: Console (Use netcat)
EXPOSE 5683 5684 5685 5686

ENTRYPOINT ["/root/LostPuppy"]
CMD ["--console"]
