# Build stage
FROM alpine:3.20 AS build

RUN apk add --no-cache build-base cmake git linux-headers
WORKDIR /src
COPY . .

RUN cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --target todo-server --config Release

# Runtime stage
FROM alpine:3.20

# ↓↓↓ Přidej potřebné knihovny pro dynamicky linkovanou C++ binárku ↓↓↓
RUN apk add --no-cache libstdc++ libgcc

WORKDIR /app
COPY --from=build /src/build/todo-server .
EXPOSE 18080
CMD ["./todo-server"]
