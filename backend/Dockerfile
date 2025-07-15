# Build stage
FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /src

# ────────────────┤ FÁZE 1: Cache Dependencies ├───────────────
# Kopíruj pouze CMakeLists.txt pro stažení závislostí
COPY CMakeLists.txt ./

# Vytvoř prázdné src a tests složky, aby cmake našel co potřebuje
RUN mkdir -p src tests

# Vytvoř prázdný main.cpp, aby cmake mohl běžet
RUN echo "int main(){return 0;}" > src/main.cpp

# Stáhni všechny závislosti (tohle se cachuje!)
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release

# ────────────────┤ FÁZE 2: Build Real App ├───────────────
# Smaž dummy soubory a zkopíruj skutečné zdrojáky
RUN rm -rf src tests

# Zkopíruj skutečné zdrojáky
COPY src/ ./src/
COPY tests/ ./tests/

# Znovu konfiguruj s pravými soubory (rychlé, závislosti už jsou stažené)
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release

# Build aplikaci
RUN cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app
COPY --from=build /src/build/todo-server .
EXPOSE 18080
CMD ["./todo-server"]