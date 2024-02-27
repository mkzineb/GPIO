
HEADERS=csim.h
SOURCES=csim.c mem.c
OBJECTS=$(SOURCES:.c=.o)

CFLAGS=-g3 -Wall -fPIC

all: test-csim libcsim.a

clean:
	-rm -rf $(OBJECTS) test-csim.o test-csim

test-csim: test-csim.o libcsim.a
	$(CC) -o $@ $^ -L. -lcsim

libcsim.a: $(OBJECTS)
	ar rcs $@ $(OBJECTS)

csim.o: csim.h
test-csim.o: csim.h
