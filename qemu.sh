#!/bin/bash

set -e
. ./iso.sh

mkdir -p ./log
logfile=$(date +%F_%H:%M:%S).log

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom nanos.iso -serial file:./log/$logfile