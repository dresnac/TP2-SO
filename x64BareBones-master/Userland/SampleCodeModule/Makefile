include ../Makefile.inc

# MODULE=0000-sampleCodeModule.bin
# SOURCES=$(wildcard [^_]*.c)
# SOURCES_ASM=$(wildcard asm/*.asm)
# OBJECTS=$(SOURCES:.c=.o)
# OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

# all: $(MODULE)

# $(MODULE): $(SOURCES)
# 	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) -o ../$(MODULE)

# clean:
# 	rm -rf asm/*.o c/*.o *.o

# .PHONY: all clean print

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)

LOADERSRC=_loader.c
LOADEROBJECT=$(LOADERSRC:.c=.o)

INCLUDE_DIRS=$(shell find include -type d) $(shell find ../../Shared/include -type d)
INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS))

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)

clean:
	rm -rf */*.o *.o *.bin 
	rm -rf *.elf

.PHONY: all clean print
