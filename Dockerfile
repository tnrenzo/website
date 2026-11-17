FROM golang:1.25-alpine AS build

WORKDIR /src

COPY go.mod .
COPY main.go .

RUN CGO_ENABLED=0 go build -ldflags="-s -w" -o /server .

FROM scratch

COPY --from=build /server /server
COPY public /public

WORKDIR /

EXPOSE 8080

ENTRYPOINT ["/server"]
