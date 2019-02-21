include ../Makefile.rules

OBJDIR = $(BUILDDIR)/kernel/
DEPDIR = $(BUILDDIR)/kernel/

CFLAGS = -std=c11 -ffreestanding -O2 -Wall -Wextra
CXXFLAGS = -std=c++14 -ffreestanding -O2 -Wall -Wextra
ASMFLAGS = 

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
-include $(CXXOBJ:.o=.d)

kernel: $(COBJ) $(CXXOBJ) $(ASMOBJ)
	$(CXX) -T linker.ld -o $(BUILDDIR)/kernel.bin -ffreestanding -O2 -nostdlib -lgcc $(COBJ) $(CXXOBJ) $(ASMOBJ) 

.PHONY: init
init:
	mkdir -p $(OBJDIR)

dep: 

$(OBJDIR)/%.o:%.s
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJDIR)/%.o:%.c
	$(CC) -S $(CFLAGS) $< -o $(OBJDIR)/$*.s
	$(CC) -c $(CFLAGS) $(OBJDIR)/$*.s -o $@
	$(CC) -MM $(CFLAGS) $< > $(OBJDIR)/$*.d

$(OBJDIR)/%.o:%.cpp
	$(CXX) -S $(CXXFLAGS) $< -o $(OBJDIR)/$*.s
	$(CXX) -c $(CXXFLAGS) $(OBJDIR)/$*.s -o $@
	$(CXX) -MM $(CXXFLAGS) $< > $(OBJDIR)/$*.d
