# syntax=docker/dockerfile:1.7

# ========= Toolchain base =========
FROM alpine:3.22 AS base
RUN apk add --no-cache \
    build-base cmake ninja git curl unzip bash linux-headers ccache
WORKDIR /app

# ========= Deps =========
FROM base AS deps
WORKDIR /deps

COPY CMakeLists.txt ./
COPY cmake/ cmake/

# 1) Configure
RUN --mount=type=cache,target=/root/.cache/ccache \
    mkdir -p src && echo 'void dummy() {}' > src/dummy.cpp && \
    cmake -B build -S . -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

# 2) Explicitně napopuluj FetchContent dependency zdroje
#    (názvy vycházejí z logu: json-populate, asio-populate, crow-populate)
RUN cmake --build build --target json-populate asio-populate crow-populate --parallel

# ========= Build =========
FROM base AS build
WORKDIR /app
# zkopírujeme celé /deps vč. build/_deps
COPY --from=deps /deps /deps

# vlastní zdrojáky
COPY CMakeLists.txt ./
COPY cmake/ cmake/
COPY src/ src/

RUN --mount=type=cache,target=/root/.cache/ccache \
    cmake -B build -S . -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
      -DFETCHCONTENT_BASE_DIR=/deps/build/_deps \
      -DFETCHCONTENT_UPDATES_DISCONNECTED=ON \
  && cmake --build build --target todo-server --parallel \
  && strip build/todo-server || true

# ========= Final (runtime) =========
FROM alpine:3.22 AS final
RUN apk add --no-cache libstdc++ ca-certificates
WORKDIR /opt/app

# Z build stage už jen binárku
COPY --from=build /app/build/todo-server /opt/app/bin/todo-server

# Ne-root uživatel
RUN adduser -D -H app && chown -R app:app /opt/app
USER app

CMD ["/opt/app/bin/todo-server"]
