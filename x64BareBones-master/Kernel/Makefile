include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_ADTS=$(wildcard ADTs/*.c)
SOURCES_TESTS=$(wildcard tests/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_SHARED=$(SOURCES_SHARED:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_ADTS=$(SOURCES_ADTS:.c=.o)
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

INCLUDE_DIRS=$(shell find include -type d) $(shell find ../Shared/include -type d)
INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS))

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_ADTS) $(OBJECTS_TESTS) $(OBJECTS_SHARED)
	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_ADTS) $(OBJECTS_TESTS) $(OBJECTS_SHARED)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_ADTS) $(OBJECTS_TESTS) $(OBJECTS_SHARED)


%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean
