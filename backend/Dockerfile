# Build stage
FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /src

# ────────────────┤ KROK 1: Kopíruj CMakeLists.txt ├───────────────
COPY CMakeLists.txt ./

# ────────────────┤ KROK 2: Vytvoř minimální strukturu pro cmake configure ├───────────────
# CMakeLists.txt používá file(GLOB_RECURSE) - potřebuje existující složky
RUN mkdir -p src tests

# ────────────────┤ KROK 3: Kopíruj jen main.cpp pro configure ├───────────────
# To stačí, aby cmake configure prošel a stáhl závislosti
COPY src/main.cpp ./src/

# ────────────────┤ KROK 4: Konfiguruj projekt a stáhni závislosti ├───────────────
# Tohle stáhne Crow, ASIO, nlohmann::json a GoogleTest
# Tato vrstva se cachuje, dokud se nezmění CMakeLists.txt!
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release

# ────────────────┤ KROK 5: Teď zkopíruj všechny zdrojáky a testy ├───────────────
# Tyto soubory se mění často, ale závislosti už jsou stažené a cacheované
COPY src/ ./src/
COPY tests/ ./tests/

# ────────────────┤ KROK 6: Znovu konfiguruj s kompletními soubory ├───────────────
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release

# ────────────────┤ KROK 7: Build aplikace ├───────────────
# Závislosti se už nemusí stahovat znovu!
RUN cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app
COPY --from=build /src/build/todo-server .
EXPOSE 18080
CMD ["./todo-server"]