SHELL = /bin/sh

srcdir = .

sources = main.c

.SUFFIXES:
.SUFFIXES: .c .o

CC = gcc -O -std=c11
DEFS =
CDEBUG = -g
WFLAGS = -Wall -Wpedantic -Wextra -Werror
CFLAGS = $(CDEBUG) -I. -I$(srcdir) $(DEFS) $(WFLAGS)
LDFLAGS = $(CDEBUG)
LDLIBS =

.PHONY: all
all: txtbrowse

objects = $(sources:.c=.o)

txtbrowse: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(sources:.c=.d)

.PHONY: clean
clean:
	-rm -f txtbrowse *.o *.d
