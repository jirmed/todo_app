FROM alpine:3.22 AS base

# Definice argumentů pro verzování
ARG BUILD_VERSION="dev"
ARG GIT_SHA="unknown"

RUN apk update \
