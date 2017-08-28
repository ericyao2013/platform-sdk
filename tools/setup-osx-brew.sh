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

ln -s /usr/local/opt/openssl/lib/libcrypto.1.0.0.dylib /usr/local/lib/
ln -s /usr/local/opt/openssl/lib/libssl.1.0.0.dylib /usr/local/lib/