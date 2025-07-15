# syntax=docker/dockerfile:1.7  # BuildKit syntax pro cache‑mount

################################################################################
# STAGE 0 – společný základ se všemi build nástroji + ccache
################################################################################
FROM alpine:3.20 AS build-base

# build-base obsahuje kompilátory, CMake, Ninja a ccache
RUN apk add --no-cache \
        build-base \
        cmake \
        ninja \
        git \
        linux-headers \
        ccache \
    && ccache -M 150M   # velikost cache ~150MB

ENV CCACHE_DIR=/ccache
ENV CMAKE_GENERATOR=Ninja
# rychlejší paralelní build

################################################################################
# STAGE 1 – závislosti, které se mění zřídka (Crow, Asio, nlohmann::json, …)
################################################################################
FROM build-base AS deps

WORKDIR /deps

# Zkopíruj pouze soubory, které určují VERZE závislostí.
# Při změně vlastního kódu zůstanou tyto vrstvy v cache.
COPY CMakeLists.txt .

# Build pouze dependencies – žádný aplikační kód.
# Využívá BuildKit cache‑mount pro uložení ccache artefaktů.
RUN --mount=type=cache,target=/ccache \
    cmake -Bbuild \
          -DCMAKE_BUILD_TYPE=Release \
          -DTODO_BUILD_DEPS_ONLY=ON && \
    cmake --build build --parallel && \
    cmake --install build --prefix /usr/local

# Pozn.: proměnná TODO_BUILD_DEPS_ONLY je volitelná.  Můžete:
#  a) zavést ve vlastním CMakeLists.txt (viz komentáře níže),
#  b) anebo zde místo toho explicitně postavit konkrétní target Crow atd.

################################################################################
# STAGE 2 – build vlastního backendu proti již připraveným knihovnám
################################################################################
FROM build-base AS app-build

WORKDIR /src

# Přetáhni už zbuildované knihovny z předchozího stage
COPY --from=deps /usr/local /usr/local

# Zbytek projektu (zdrojáky, testy, …)
COPY CMakeLists.txt .
COPY src/ src/
COPY tests/ tests/

# Vlastní build (ccache mount opět urychluje inkrementální překlady)
RUN --mount=type=cache,target=/ccache \
    cmake -Bbuild \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_C_COMPILER_LAUNCHER=ccache \
          -DCMAKE_CXX_COMPILER_LAUNCHER=ccache && \
    cmake --build build --target todo-server --parallel && \
    strip build/todo-server

################################################################################
# STAGE 3 – minimální runtime image
################################################################################
FROM alpine:3.20

RUN apk add --no-cache libstdc++ libgcc

WORKDIR /app
COPY --from=app-build /src/build/todo-server .

EXPOSE 18080
ENTRYPOINT ["./todo-server"]
