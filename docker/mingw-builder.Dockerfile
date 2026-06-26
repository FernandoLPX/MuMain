FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
ARG LIBJPEG_TURBO_VERSION=2.1.5.1

RUN apt-get update && apt-get install -y \
    ca-certificates \
    cmake \
    ninja-build \
    mingw-w64 \
    g++-mingw-w64-i686 \
    gcc-mingw-w64-i686 \
    git \
    curl \
    xz-utils \
    make \
    nasm \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt

# Build static libturbojpeg.a for i686-w64-mingw32 once in the image.
RUN curl -fsSL -o libjpeg-turbo.tar.gz \
      "https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/${LIBJPEG_TURBO_VERSION}.tar.gz" \
    && tar -xzf libjpeg-turbo.tar.gz \
    && cmake -S "libjpeg-turbo-${LIBJPEG_TURBO_VERSION}" -B libjpeg-turbo-build -G Ninja \
      -DCMAKE_SYSTEM_NAME=Windows \
      -DCMAKE_SYSTEM_PROCESSOR=X86 \
      -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc \
      -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ \
      -DCMAKE_RC_COMPILER=i686-w64-mingw32-windres \
      -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_SHARED=FALSE \
      -DENABLE_STATIC=TRUE \
      -DCMAKE_INSTALL_PREFIX=/opt/mingw-turbo \
    && cmake --build libjpeg-turbo-build -j"$(nproc)" \
    && cmake --install libjpeg-turbo-build \
    && test -f /opt/mingw-turbo/lib/libturbojpeg.a \
    && rm -rf /opt/libjpeg-turbo.tar.gz /opt/libjpeg-turbo-${LIBJPEG_TURBO_VERSION} /opt/libjpeg-turbo-build

WORKDIR /work

CMD ["bash", "-lc", "cmake -S . -B build-mingw -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-w64-i686.cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_EDITOR=OFF -DMU_TURBOJPEG_STATIC_LIB=/opt/mingw-turbo/lib/libturbojpeg.a && cmake --build build-mingw -j$(nproc)"]
