# FILE: Dockerfile
# -------- Base toolchain image --------
FROM alpine:3.22 AS base

RUN apk update \
    && apk add --no-cache \
       build-base \
       curl \
       git \
       unzip \
       musl-dev \
       openssl \
       bash \
       cmake \
       linux-headers \
    && rm -rf /var/cache/apk/*

WORKDIR /app

# -------- Deps stage (cacheable FetchContent) --------
FROM base AS deps

# CMake files and project root CMakeLists
COPY cmake/ cmake/
COPY CMakeLists.txt .
# IMPORTANT: include/ is needed at configure time for build_info.hpp.in
COPY include/ include/

# Create minimal src to allow configure step to run
RUN mkdir -p src tests && echo 'void dummy() {}' > src/dummy.cpp

# Configure to fetch dependencies into build/_deps (no build yet)
RUN cmake -B build -S . -DBUILD_TESTING=OFF

# -------- Build stage (application only, no tests) --------
FROM base AS build
WORKDIR /app

COPY --from=deps /app/build/_deps/ /app/build/_deps/
COPY src/ src/
COPY include/ include/
COPY CMakeLists.txt .
COPY cmake/ cmake/

# Build args + env (přeneseme i do ENV, ať funguje fallback přes $ENV{...} v CMake)
ARG APP_VERSION
ARG VCS_REF
ARG GIT_TAG
ENV APP_VERSION=${APP_VERSION}
ENV VCS_REF=${VCS_REF}
ENV GIT_TAG=${GIT_TAG}

# Cache-bust: změna vrstvy při každém commitu
RUN echo "${VCS_REF}" > .vcs_ref

# Robustní volání CMake – vždy pošli -D, i když je hodnota prázdná
RUN cmake -B build -S . -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release \
    -DAPP_VERSION="${APP_VERSION}" \
    -DGIT_SHA="${VCS_REF}" \
    -DGIT_TAG="${GIT_TAG}" \
 && cmake --build build --target todo-server --config Release -j

# ... (final stage beze změny, ale ponech ARG/labels)
FROM alpine:3.22 AS final
RUN apk add --no-cache libstdc++ curl
WORKDIR /app
COPY --from=build /app/build/todo-server .

ARG APP_VERSION
ARG VCS_REF
ARG BUILD_DATE
LABEL org.opencontainers.image.title="todo_app" \
      org.opencontainers.image.version="${APP_VERSION}" \
      org.opencontainers.image.revision="${VCS_REF}" \
      org.opencontainers.image.created="${BUILD_DATE}" \
      org.opencontainers.image.source="https://github.com/jirmed/todo_app"

CMD ["./todo-server"]
