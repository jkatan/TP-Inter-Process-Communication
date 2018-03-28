include Makefile.inc

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
DEPS=$(wildcard include/*.h)
SLAVEPROCESS=slaveProcess.o

all:$(SLAVEPROCESS)

$(SLAVEPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES) $(GCCFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean
