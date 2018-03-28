include Makefile.inc


OBJECTS=$(SOURCES:.c=.o)
DEPS=$(wildcard include/*.h)
QUEUETEST=queueTest.o
SOURCE_QUEUETEST=queueTest.c
SLAVEPROCESSTEST=slaveProcessTest.o
SOURCE_SLAVEPROCESSTEST=slaveProcessTest.c

OTHERDEPS=$(wildcard ../MD5_Calculator/*.c)

all:$(SLAVEPROCESS) $(QUEUETEST)

$(SLAVEPROCESSTEST): $(DEPS)
	$(GCC)  -o $@ $(SOURCE_QUEUETEST) $(OTHERDEPS) $(GCCFLAGS)
$(QUEUETEST): $(DEPS)
	$(GCC)  -o $@ $(SOURCE_SLAVEPROCESSTEST) $(OTHERDEPS) $(GCCFLAGS)

clean:
	rm -rf *.o

.PHONY: all clean
