include Makefile.inc


DEPS=$(wildcard include/*.h)
DEPS_IN_OTHER_FOLDER=$(wildcard ../MD5_Calculator/include/*.h)

SLAVEPROCESSTEST=slaveProcessTest
SOURCES_SLAVEPROCESSTEST=slaveProcessTest.c ../MD5_Calculator/queuelib.c ../MD5_Calculator/slaveProcessLib.c ../MD5_Calculator/hashedfile.c  testlib.c

QUEUETEST=queueTest
SOURCES_QUEUETEST=queueTest.c queuelib.c testlib.c



all:$(SLAVEPROCESS) $(QUEUETEST)

$(SLAVEPROCESSTEST): $(DEPS) $(DEPS_IN_OTHER_FOLDER)
	$(GCC)  -o $@ $(SOURCES_QUEUETEST) $(GCCFLAGS)
$(QUEUETEST): $(DEPS) $(DEPS_IN_OTHER_FOLDER)
	$(GCC)  -o $@ $(SOURCES_SLAVEPROCESSTEST) $(GCCFLAGS)

clean:
	rm -rf *.o $(SLAVEPROCESSTEST) $(QUEUETEST)

.PHONY: all clean
