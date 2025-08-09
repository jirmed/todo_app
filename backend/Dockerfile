# syntax=docker/dockerfile:1.7

# ========= Toolchain base =========
FROM alpine:3.22 AS base
RUN apk add --no-cache \
    build-base cmake ninja git curl unzip bash linux-headers ccache

WORKDIR /app

# ========= Deps (configure + FetchContent) =========
FROM base AS deps

COPY CMakeLists.txt ./
COPY cmake/ cmake/

RUN --mount=type=cache,target=/root/.cache/ccache \
    mkdir -p src && echo 'void dummy() {}' > src/dummy.cpp && \
    cmake -B build -S . \
      -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

# ========= Build (kompilace app) =========
FROM base AS build
WORKDIR /app

COPY --from=deps /app/build /app/build
COPY --from=deps /app/cmake /app/cmake
COPY --from=deps /app/CMakeLists.txt /app/CMakeLists.txt

COPY src/ src/

RUN --mount=type=cache,target=/root/.cache/ccache \
    cmake -B build -S . \
      -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
      -DCMAKE_INSTALL_PREFIX=/opt/app \
 && cmake --build build --target install \
 && strip /opt/app/bin/todo-server || true

# ========= Final (runtime) =========
FROM alpine:3.22 AS final
RUN apk add --no-cache libstdc++ ca-certificates
WORKDIR /opt/app
COPY --from=build /opt/app /opt/app

RUN adduser -D -H app && chown -R app:app /opt/app
USER app

CMD ["/opt/app/bin/todo-server"]
