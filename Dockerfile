FROM golang:1.25-alpine AS build

WORKDIR /app

COPY go.mod main.go ./

RUN CGO_ENABLED=0 go build \
    -trimpath \
    -ldflags="-s -w" \
    -o /server .

FROM emscripten/emsdk:7.0.0 AS build-wasm

# working checkout hash
ARG RAYLIB_COMMIT=0dedaf3066ffd0a63b5899385f59055a915f839c

WORKDIR /raylib

RUN git init -q . \
    && git remote add origin https://github.com/raysan5/raylib.git \
    && git fetch -q --depth 1 origin "$RAYLIB_COMMIT" \
    && git checkout -q FETCH_HEAD \
    && make -C src PLATFORM=PLATFORM_WEB -j"$(nproc)"

WORKDIR /app

COPY public/magnetic-field/native/main.c ./

RUN mkdir out && emcc main.c \
    -Os \
    -DPLATFORM_WEB \
    -I /raylib/src \
    /raylib/src/libraylib.web.a \
    -s USE_GLFW=3 \
    -o out/magnetic-field.js

FROM scratch

WORKDIR /app

COPY --from=build /server /server
COPY public ./public
COPY --from=build-wasm /app/out/ ./public/magnetic-field/

USER 65532:65532

EXPOSE 8080

ENV ADDR=0.0.0.0:8080

ENTRYPOINT ["/server"]
