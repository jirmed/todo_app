FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /src

# Cache CMakeLists.txt - změní se velmi zřídka
COPY CMakeLists.txt ./

# Cache strukturu složek - změní se zřídka
RUN mkdir -p src tests

# Kopíruj celý zdrojový kód najednou
COPY src/ ./src/
COPY tests/ ./tests/

# Jediný cmake configure + build
RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20
RUN apk add --no-cache libstdc++ libgcc
WORKDIR /app
COPY --from=build /src/build/todo-server .
EXPOSE 18080
CMD ["./todo-server"]