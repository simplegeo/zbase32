#!/usr/bin/env python
#
# Copyright (c) 2002-2007 Zooko "Zooko" Wilcox-O'Hearn
# mailto:zooko@zooko.com
#
# Permission is hereby granted to any person obtaining a copy of this work to
# deal in this work without restriction (including the rights to use, modify,
# distribute, sublicense, and/or sell copies).

NAME=base32

INCDIRS=-I../libzutil -I../libzstr
LIBDIRS=-L../libzutil -L../libzstr
LIBS=-lzstr -lzutil -lm

LIBPREFIX=lib
LIBSUFFIX=.a

RANLIB=ranlib
AR=ar

# CC=gcc

CFLAGS=-DNDEBUG -Wall -O0 $(INCDIRS)
LDFLAGS=$(LIBDIRS) $(LIBS)

# SRCS=$(wildcard *.c)
SRCS=base32.c
TESTSRCS=test.c
OBJS=$(SRCS:%.c=%.o)
TESTOBJS=$(TESTSRCS:%.c=%.o)
TEST=testlb32
LIB=$(LIBPREFIX)$(NAME)$(LIBSUFFIX)

all: $(LIB) $(TEST)

include $(SRCS:%.c=%.d)

%.d: %.c
	@echo remaking $@
	@set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

$(LIB): $(OBJS)
	$(AR) -r $@ $+
	$(RANLIB) $@

$(TEST): $(TESTOBJS) $(LIB)
	$(CC) $+ -o $@ $(LDFLAGS)


clean:
	-rm $(LIB) $(OBJS) $(TEST) $(TESTOBJS) *.d

.PHONY: clean all
