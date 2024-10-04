FROM alpine:3.20.1

RUN apk add ninja-build samurai autoconf automake git cmake compiler-rt

RUN apk add gcc g++ python3 py3-pip curl zip unzip tar

RUN git clone https://github.com/microsoft/vcpkg /usr/local/share/vcpkg && \
    /usr/local/share/vcpkg/bootstrap-vcpkg.sh -disableMetrics

ENV VCPKG_ROOT=/usr/local/share/vcpkg