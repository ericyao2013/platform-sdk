#!/bin/ash
set -ex

apk update

apk del --purge build-base cmake pkgconfig boost-dev libressl-dev protobuf-dev
