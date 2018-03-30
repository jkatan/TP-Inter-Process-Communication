include Makefile.inc

SOURCES=$(wildcard *.c)
SOURCES_APPLICATIONPROCESS = $(filter-out slaveProcess.c,$(SOURCES))
SOURCES_SLAVEPROCESS = $(filter-out applicationProcess.c,$(SOURCES))
OBJECTS=$(SOURCES:.c=.o)
DEPS=$(wildcard include/*.h)
SLAVEPROCESS=slaveProcess
APPLICATIONPROCESS=applicationProcess

all:$(SLAVEPROCESS) $(APPLICATIONPROCESS)

$(SLAVEPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES_SLAVEPROCESS) $(GCCFLAGS)

$(APPLICATIONPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES_APPLICATIONPROCESS) $(GCCFLAGS)

clean:
	rm -rf *.o $(SLAVEPROCESS) $(APPLICATIONPROCESS)

.PHONY: all clean
