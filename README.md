# airmapd

## Development Workflow

`airmapd` uses CMake for building and testing. We envision the following development workflow:
```
# Please execute these commands at the root of the source tree
mkdir build
cd build && cmake .. && make
# Do some changes, iterate, be happy, get ready to issue a PR
make format
```

## Setup & Dependencies:

Please refer to the `setup-*.sh` scripts in the `tools/` subfolder. For Ubuntu, and under the assumption of `docker` being available,
you can bootstrap a development environment in a `docker` container with:
```
docker run -v $(PWD):/airmapd -it ubuntu:16.04 bash
tools/setup-ubuntu.sh
```
