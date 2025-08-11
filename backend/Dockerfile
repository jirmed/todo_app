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

# Reuse fetched dependencies from deps stage (keep cache hot)
COPY --from=deps /app/build/_deps/ /app/build/_deps/

# Project sources
COPY src/ src/
COPY include/ include/
COPY CMakeLists.txt .
COPY cmake/ cmake/

# Build-time metadata (optional; only pass when available)
ARG APP_VERSION

# Configure & build (pass APP_VERSION only when non-empty)
RUN cmake -B build -S . -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release \
    $([ -n "$APP_VERSION" ] && echo -DAPP_VERSION=${APP_VERSION}) \
 && test -f build/generated/todo/build/build_info.hpp && echo "build_info.hpp OK" \
 && cmake --build build --target todo-server --config Release -j

# -------- Final runtime image --------
FROM alpine:3.22 AS final

# Minimal runtime deps
RUN apk add --no-cache libstdc++ curl

WORKDIR /app
COPY --from=build /app/build/todo-server .

# OCI labels (for audit/rollback)
ARG APP_VERSION
ARG VCS_REF
ARG BUILD_DATE
LABEL org.opencontainers.image.title="todo_app" \
      org.opencontainers.image.version="${APP_VERSION}" \
      org.opencontainers.image.revision="${VCS_REF}" \
      org.opencontainers.image.created="${BUILD_DATE}" \
      org.opencontainers.image.source="https://github.com/jirmed/todo_app"

CMD ["./todo-server"]
