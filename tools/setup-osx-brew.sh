#!/bin/bash

set -ex

brew update

brew install \
  cmake \
  boost \
  cryptopp \
  openssl \
  pkgconfig \
  protobuf
