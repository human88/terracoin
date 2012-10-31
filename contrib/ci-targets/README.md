# ci-targets #

This directory contains various scripts used by our continuous integration servers
for building terracoin binaries for linux and windows 32+64 bits platforms under linux.

## Requirements ##

we use debian machines for our builders, but any linux distribution may be used.

apt-get install gcc-mingw32 mingw32-binutils mingw32-runtime mingw-w64 gcc g++ zip


## Scripts ##

### 1.env-setup.sh ###

This script fetches project dependencies source files once,
storing them in a directory where following build steps scripts
may get them for building.


