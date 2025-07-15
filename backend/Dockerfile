# Build stage
FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /src

# Kopíruj nejdřív závislosti a cmake konfigurace pro lepší cache
COPY CMakeLists.txt ./
COPY src/CMakeLists.txt ./src/
# Pokud máš další soubory závislostí, přidej je zde (např. vcpkg.json apod.)

# Spusť cmake pouze pro konfiguraci
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release

# Teď přidej zbytek zdrojových souborů
COPY src/ ./src/

RUN cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app
COPY --from=build /src/build/todo-server . 
EXPOSE 18080
CMD ["./todo-server"]