##
## This file is part of the flosslogic project.
##
## Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
##

PROGRAM = flosslogic

PREFIX ?= /usr/local
CFLAGS  = -Wall -Wextra -Werror -Wshadow -g -I../include
LDFLAGS = -L../lib
LIBS    = -lflosslogic
MANDIR ?= $(PREFIX)/share/man
OBJS    = $(PROGRAM).o cmdline.o output.o
HEADERS = common.h

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(HEADERS)
	$(CC) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(PROGRAM)

install: $(PROGRAM)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	install -m 755 $(PROGRAM) $(DESTDIR)$(PREFIX)/bin
	install -m 644 $(PROGRAM).1 $(DESTDIR)$(MANDIR)/man1

.PHONY: all clean install

