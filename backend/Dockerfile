# Build stage
FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /src

# ────────────────┤ FÁZE 1: Stažení závislostí ├───────────────
# Kopíruj pouze soubory pro závislosti
COPY cmake/ ./cmake/

# Stáhni pouze závislosti (bez zdrojáků)
RUN cmake -S cmake -B build-deps -f deps-only.cmake -DCMAKE_BUILD_TYPE=Release

# ────────────────┤ FÁZE 2: Build aplikace ├───────────────
# Teď zkopíruj vše ostatní
COPY CMakeLists.txt ./
COPY src/ ./src/
COPY tests/ ./tests/

# Použij existující cache se stažtenými závislostmi
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app
COPY --from=build /src/build/todo-server .
EXPOSE 18080
CMD ["./todo-server"]