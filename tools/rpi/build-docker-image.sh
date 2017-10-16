#!/bin/bash

set -ex

build_opts="--force-rm=true $@"
sudo docker run --rm --privileged multiarch/qemu-user-static:register --reset
sudo docker build ${build_opts} -t airmapd:latest -f Dockerfile.rpi .
