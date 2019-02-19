include Makefile.rules


DIRS = kernel

all: subdirs


burnimage: subdirs
	

runkernel: subdirs
	screen -dmS OsDev qemu-system-i386 -kernel build/kernel.bin

subdirs: init
	for i in $(DIRS); do $(MAKE) -C $$i; done

.PHONY: init
init: 
	mkdir -p build

.PHONY: clean
clean:  
	rm -rf build
