CFLAGS=-I../include -I../src -g -O3
LIBADD =  $(shell bash ../src/toy-config --libs)
EXEC=toy-sim$(EXE_SUFFIX)

all: $(EXEC)

$(EXEC): toy-sim.o  ../src/libtoy.a
	$(CC) $(CFLAGS) -o $@ $< $(LIBADD)

clean:
	rm -f toy-sim.o

distclean: clean
	rm -f $(EXEC)
