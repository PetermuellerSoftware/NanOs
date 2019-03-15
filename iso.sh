#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/nanos.kernel isodir/boot/nanos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "nanos" {
	multiboot /boot/nanos.kernel
}
EOF
grub-mkrescue -o nanos.iso isodir