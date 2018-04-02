include Makefile.inc

SOURCES=$(wildcard *.c)
SOURCES_APPLICATIONPROCESS = $(filter-out slaveProcess.c viewProcess.c,$(SOURCES))
SOURCES_SLAVEPROCESS = $(filter-out applicationProcess.c viewProcess.c,$(SOURCES))
SOURCES_VIEWPROCESS = $(filter-out applicationProcess.c slaveProcess.c,$(SOURCES))
OBJECTS=$(SOURCES:.c=.o)
DEPS=$(wildcard include/*.h)
SLAVEPROCESS=slaveProcess
APPLICATIONPROCESS=../applicationProcess
VIEWPROCESS=../viewProcess

all:$(SLAVEPROCESS) $(APPLICATIONPROCESS) $(VIEWPROCESS)

$(SLAVEPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES_SLAVEPROCESS) $(GCCFLAGS)

$(APPLICATIONPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES_APPLICATIONPROCESS) $(GCCFLAGS)

$(VIEWPROCESS): $(DEPS)
	$(GCC)  -o $@ $(SOURCES_VIEWPROCESS) $(GCCFLAGS)

clean:
	rm -rf *.o $(SLAVEPROCESS) $(APPLICATIONPROCESS)

.PHONY: all clean
