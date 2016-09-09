FROM alpine:3.3
RUN apk add --no-cache --update make g++ bash git linux-headers

RUN mkdir -p /root/
WORKDIR /root

COPY lib lib
COPY downloadDeps.sh downloadDeps.sh
COPY main.cpp main.cpp
COPY Makefile Makefile

RUN chmod +x downloadDeps.sh && ./downloadDeps.sh

ENV SECURE=1 DEBUG=1 OIC_SERVER=1

RUN make

EXPOSE 5683 5684 5685 5686

ENTRYPOINT ["neudev"]
CMD ["--console"]
