all: md5_calculator tests

fromzero: clean all; make all;

allTests:
			clean all; cd Tests; ./queueTest; ./slaveProcess ;

md5_calculator:
			cd MD5_Calculator; make all

tests:
			cd Tests; make all

clean:
			cd MD5_Calculator; make clean
			cd tests; make clean

.PHONY: tests all allTests tests md5_calculator clean
