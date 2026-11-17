FROM golang:1.25-alpine AS build

WORKDIR /app

COPY go.mod main.go ./

RUN CGO_ENABLED=0 go build \
    -trimpath \
    -ldflags="-s -w" \
    -o /server .

FROM scratch

WORKDIR /app

COPY --from=build /server /server
COPY public ./public

USER 65532:65532

EXPOSE 8080

ENV ADDR=0.0.0.0:8080

ENTRYPOINT ["/server"]
