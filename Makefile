SHELL = /bin/sh

srcdir = .

sources = window.c splay.c hash.c qsort.c mergesort.c

.SUFFIXES:
.SUFFIXES: .c .o

CC = gcc -O3 -std=c11
DEFS =
CDEBUG = -g
WFLAGS = -Wall -pedantic-errors -Wextra
CFLAGS = $(CDEBUG) -I. -I$(srcdir) $(DEFS) $(WFLAGS)
LDFLAGS = $(CDEBUG)
LDLIBS =

.PHONY: all
all: txtbrowse test

objects = $(sources:.c=.o)

txtbrowse: $(objects) main.o
	$(CC) $(LDFLAGS) -o $@ $(objects) main.o $(LDLIBS)

test: $(objects) test.o
	$(CC) $(LDFLAGS) -o $@ $(objects) test.o $(LDLIBS)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(sources:.c=.d) main.d test.d

.PHONY: clean
clean:
	-rm -f txtbrowse *.o *.d
