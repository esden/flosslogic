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

LIBNAME   = flosslogic

OS        = $(shell uname)
ifeq ($(findstring MINGW,$(OS)),MINGW)
	# OS is of the form MINWG32_XXXX, replace it with plain "MINGW".
	OS = MINGW
endif
CC        = gcc
INSTALL  ?= install
PREFIX   ?= /usr/local
CFLAGS   += -g -Wall -Wextra -Werror -Wshadow
CPPFLAGS += -I../include
LDFLAGS  += -L/usr/local/lib -L.
LIBS      = -lusb
SRC       = libflosslogic.c libusb.c fx2.c output.c hw_usbeesx.c hw_lps.c
HEADERSS  = ../include/flosslogic.h common.h

ifeq ($(OS),Darwin)
SONAME    = lib$(LIBNAME).dylib
else ifeq ($(OS),MINGW)
SONAME    = lib$(LIBNAME).a
else
SONAME    = lib$(LIBNAME).so
endif

all: $(SONAME)

$(SONAME): $(SRC) $(HEADERS)
ifeq ($(OS),Darwin)
	$(CC) -dynamiclib -flat_namespace -undefined suppress \
		-o lib$(LIBNAME).dylib $(SRC) \
		$(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS)
else ifeq ($(OS),MINGW)
	$(CC) -shared -o $(SONAME) $(SRC) \
		$(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS)
else
	$(CC) -shared -Wl,-soname,lib$(LIBNAME).so.1 \
		-o lib$(LIBNAME).so $(SRC) \
		$(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS)
	ln -sf lib$(LIBNAME).so lib$(LIBNAME).so.1
	ln -sf lib$(LIBNAME).so.1 lib$(LIBNAME).so.1.0.0
endif

clean:
	rm -f *.o *.so *.so.1 *.so.1.0.0 *.dylib *.dll *.dll.a *.a

install: lib$(LIBNAME).so
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	$(INSTALL) -m 644 ../include/$(LIBNAME).h $(PREFIX)/include
	$(INSTALL) -m 755 lib$(LIBNAME).so $(PREFIX)/lib/lib$(LIBNAME).so
	ln -sf lib$(LIBNAME).so $(PREFIX)/lib/lib$(LIBNAME).so.1
	ln -sf lib$(LIBNAME).so.1 $(PREFIX)/lib/lib$(LIBNAME).so.1.0.0

.PHONY: all clean install

