all: test

CFLAGS += -O2 -g

test: test.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

test.o: test.c cmap.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	rm -f test *.o