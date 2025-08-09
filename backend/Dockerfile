# syntax=docker/dockerfile:1.7

# ========= Toolchain base =========
FROM alpine:3.22 AS base
RUN apk add --no-cache \
    build-base cmake ninja git curl unzip bash linux-headers ccache
WORKDIR /app

# ========= Deps: stáhni a připrav FetchContent jen jednou =========
FROM base AS deps
# Místo, kde bude FetchContent cache/sources
ENV CMAKE_FETCHCONTENT_BASE_DIR=/deps \
    FETCHCONTENT_UPDATES_DISCONNECTED=ON

# Jen soubory, které ovlivní závislosti
COPY CMakeLists.txt ./
COPY cmake/ cmake/

# Minimální configure, aby se napopulovaly _deps do /deps
# (header-only liby se nekompilují, jen stáhnou)
RUN --mount=type=cache,target=/root/.cache/ccache \
    mkdir -p src && echo 'void dummy() {}' > src/dummy.cpp && \
    cmake -B /deps/build -S . \
      -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache && \
    cmake --build /deps/build --parallel

# ========= Build: přelož jen aplikaci, bez instalace 3rd-party =========
FROM base AS build
WORKDIR /app

# Přenes připravené FetchContent zdroje/cache
COPY --from=deps /deps /deps
# Stejné nastavení, ale zcela "offline"
ENV CMAKE_FETCHCONTENT_BASE_DIR=/deps \
    FETCHCONTENT_FULLY_DISCONNECTED=ON \
    FETCHCONTENT_UPDATES_DISCONNECTED=ON

# Projektové metadatové soubory
COPY CMakeLists.txt ./
COPY cmake/ cmake/
# Až teď zdrojáky (neinvaliduje deps vrstvu)
COPY src/ src/

# Build pouze binárky (žádný 'install' – ušetří I/O)
RUN --mount=type=cache,target=/root/.cache/ccache \
    cmake -B build -S . \
      -G Ninja \
      -DBUILD_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_COMPILER_LAUNCHER=ccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
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
