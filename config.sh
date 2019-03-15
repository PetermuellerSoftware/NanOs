#!/bin/bash

SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

#take given make command or default "make"
export MAKE=${MAKE:-make}
#take given host or default from default-host.sh
export HOST=${HOST:-$(./default-host.sh)} 

export CROSS=${CROSS:-~/opt/cross/bin/}

# compiler paths
export AR=${CROSS}${HOST}-ar
export AS=${CROSS}${HOST}-as
export CC=${CROSS}${HOST}-gcc

#directories in target system
export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

# compiler flags
export CFLAGS='-O2 -g'
export CPPFLAGS=

# Configure the cross-compiler to use the desired system root.
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around that the -elf gcc targets doesn't have a system include directory
# because it was configured with --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi