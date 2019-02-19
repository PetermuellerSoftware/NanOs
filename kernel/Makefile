include ../Makefile.rules

OBJDIR = $(BUILDDIR)/kernel/
DEPDIR = $(BUILDDIR)/kernel/

CFLAGS = -ffreestanding -O2 -Wall -Wextra

CFILES = $(wildcard *.c)
CXXFILES = $(wildcard *.cpp)
ASMFILES = $(wildcard *.s)

COBJ = $(patsubst %.c,$(OBJDIR)/%.o, $(CFILES))
CXXOBJ = $(patsubst %.cpp, $(OBJDIR)/%.o, $(CXXFILES)) 
ASMOBJ = $(patsubst %.s,$(OBJDIR)/%.o, $(ASMFILES))

VERBOSE=false

.PHONY: all
all: init kernel

-include $(COBJ:.o=.d)

kernel: $(COBJ) $(CXXOBJ) $(ASMOBJ)
	$(CXX) -T linker.ld -o $(BUILDDIR)/kernel.bin -ffreestanding -O2 -nostdlib -lgcc $(COBJ) $(CXXOBJ) $(ASMOBJ) 

.PHONY: init
init:
	mkdir -p $(OBJDIR)

dep: 

$(OBJDIR)/%.o:%.s
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJDIR)/%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(OBJDIR)/$*.d

$(OBJDIR)/%.o:%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@
	$(CXX) -MM $(CFLAGS) $< > $(OBJDIR)/$*.d
