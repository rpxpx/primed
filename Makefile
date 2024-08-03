
CC = gcc
CFLAGS = -std=c89 -Wall -g -O0
CFLAGS = -std=c89 -Wall -O3
OBJECTS = primed.o distributions.o colorspace.o 
LDFLAGS = -lm

libprimed.a: $(OBJECTS)
	@echo "* Compiling primed library."
	ar r $@ $(OBJECTS)
	ar -s $@

ptest: primed_test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

dtest: distributions_test.o	distributions.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

ctest: colorspace_test.o colorspace.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

primed_test.o: primed.h
distributions_test.o: distributions.h
colorspace_test.o: colorspace.h
primed.o: primed.h distributions.h colorspace.h
distributions.o: distributions.h
colorspace.o: colorspace.h

.PHONY: all install clean
all: libprimed.a ptest dtest ctest
install:
	@echo "* Attempting to install libprimed to /usr/lib/, /usr/include/."
	@echo "* Might require su privilege."
	cp -i libprimed.a /usr/lib/
	cp -i *.h /usr/include/
clean:
	rm -f libprimed.a ptest dtest ctest *.o

