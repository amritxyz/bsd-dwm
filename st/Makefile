# st - simple terminal
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

SRC = st.c x.c boxdraw.c hb.c
OBJ = $(SRC:.c=.o)

all: options st

options:
	@echo st build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(STCFLAGS) -c $<

st.o: config.h st.h win.h
x.o: arg.h config.h st.h win.h hb.h
hb.o: st.h
boxdraw.o: config.h st.h boxdraw_data.h

$(OBJ): config.h config.mk

st: $(OBJ)
	$(CC) -o $@ $(OBJ) $(STLDFLAGS)

clean:
	rm -f st $(OBJ) st-$(VERSION).tar.gz *.rej *.orig *.o

dist: clean
	mkdir -p st-$(VERSION)
	cp -R FAQ LEGACY TODO LICENSE Makefile README config.mk\
		config.h st.info st.1 arg.h st.h win.h $(SRC)\
		st-$(VERSION)
	tar -cf - st-$(VERSION) | gzip > st-$(VERSION).tar.gz
	rm -rf st-$(VERSION)

install: st
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f st $(DESTDIR)$(PREFIX)/bin
	cp -f st-copyout $(DESTDIR)$(PREFIX)/bin
	cp -f st-urlhandler $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/st
	chmod 755 $(DESTDIR)$(PREFIX)/bin/st-copyout
	chmod 755 $(DESTDIR)$(PREFIX)/bin/st-urlhandler
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < st.1 > $(DESTDIR)$(MANPREFIX)/man1/st.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/st.1
	tic -sx st.info
	@echo Please see the README file regarding the terminfo entry of st.

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/st
	rm -f $(DESTDIR)$(PREFIX)/bin/st-copyout
	rm -f $(DESTDIR)$(PREFIX)/bin/st-urlhandler
	rm -f $(DESTDIR)$(MANPREFIX)/man1/st.1

.PHONY: all options clean dist install uninstall

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

PKG_CONFIG = pkg-config

# includes and libs
INCS = -I$(X11INC) \
       `$(PKG_CONFIG) --cflags fontconfig` \
       `$(PKG_CONFIG) --cflags freetype2` \
       `$(PKG_CONFIG) --cflags harfbuzz`
LIBS = -L$(X11LIB) -lm -lX11 -lutil -lXft -lXrender\
       `$(PKG_CONFIG) --libs fontconfig` \
       `$(PKG_CONFIG) --libs freetype2` \
       `$(PKG_CONFIG) --libs harfbuzz`

# flags
STCPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600
STCFLAGS = $(INCS) $(STCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
STLDFLAGS = $(LIBS) $(LDFLAGS)

# OpenBSD:
#CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
#LIBS = -L$(X11LIB) -lm -lX11 -lutil -lXft \
#       `$(PKG_CONFIG) --libs fontconfig` \
#       `$(PKG_CONFIG) --libs freetype2`

# compiler and linker
# CC = c99
