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

## Dependencies:
  * boost:
    * date-time
    * program-options
    * system
  * protobuf
  * cryptopp 
  * libsoup
  * libglib

Please use your package manager to install the dev versions of these dependencies.
