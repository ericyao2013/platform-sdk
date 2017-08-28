#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  libboost-date-time-dev \
  libboost-program-options-dev \
  libboost-system-dev \
  libprotobuf-dev \
  libcrypto++-dev \
  libssl-dev \
  pkg-config\
  protobuf-compiler
