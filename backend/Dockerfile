FROM alpine:3.22 AS base

RUN apk update \
    && apk add --no-cache \
    build-base \
    curl \
    git \
    unzip \
    musl-dev \
    openssl \
    bash \
    cmake \
    linux-headers \
    && rm -rf /var/cache/apk/*

WORKDIR /app

# 🧪 Fetch dependencies — samostatně cacheovatelná vrstva
FROM base AS deps

COPY cmake/ cmake/
COPY CMakeLists.txt .

RUN mkdir -p src tests

# Pouze stáhni závislosti (žádný --build target není potřeba)
RUN mkdir -p src && echo 'void dummy() {}' > src/dummy.cpp
RUN cmake -B build -S . -DBUILD_TESTING=OFF


# 🧩 Build project — pouze aplikace (bez testů)
FROM base AS build

WORKDIR /app

# Kopíruj dependencies z build _deps, NE z /app/_deps
COPY --from=deps /app/build/_deps/ /app/build/_deps/
COPY src/ src/
COPY CMakeLists.txt .
COPY cmake/ cmake/

RUN cmake -B build -S . -DBUILD_TESTING=OFF \
 && cmake --build build --target todo-server

# 🏁 Final image – Alpine runtime (malé)
FROM alpine:3.22 AS final

# Nutná runtime závislost (libstdc++)
RUN apk add --no-cache libstdc++ curl

WORKDIR /app
COPY --from=build /app/build/todo-server .

CMD ["./todo-server"]
