SHELL = /bin/sh

srcdir = .

objects = main.o

.SUFFIXES:
.SUFFIXES: .c .o

CC = gcc -O -std=c11
DEFS =
CDEBUG = -g
WFLAGS = -Wall -Wpedantic -Wextra -Werror
CFLAGS = $(CDEBUG) -I. -I$(srcdir) $(DEFS) $(WFLAGS)
LDFLAGS = -g
LDLIBS =
LDFLAGS =


.PHONY: all
all: txtbrowse

txtbrowse: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

.PHONY: clean
clean:
	-rm -f txtbrowse $(objects)
