include Makefile.inc

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
DEPS=$(wildcard include/*.h)
SLAVEPROCESS=slaveProcess

all:$(SLAVEPROCESS)

$(SLAVEPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES) $(GCCFLAGS)

clean:
	rm -rf *.o $(SLAVEPROCESS) 

.PHONY: all clean
