all :server client send receive zender

ifeq ($(platform),windows)
CC = i686-w64-mingw32-gcc
GTKCFLAGS = `export PKG_CONFIG_PATH=/usr/i686-w64-mingw32/sys-root/mingw/lib/pkgconfig && pkg-config --cflags gtk+-3.0 pangocairo` -pipe -pthread
GTKLIBS = `export PKG_CONFIG_PATH=/usr/i686-w64-mingw32/sys-root/mingw/lib/pkgconfig && pkg-config --libs gtk+-3.0 pangocairo`

NETWORKLIBS = -lws2_32 -liphlpapi
else
CC = gcc
GTKCFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic -pipe 
GTKLIBS = ` pkg-config --libs gtk+-3.0`
NETWORKLIBS = 
endif

CFLAGS = -g -pthread $(GTKCFLAGS)
LIBS = -Iinclude $(GTKLIBS) $(NETWORKLIBS)

vpath %.c src
vpath %.o bin
vpath %.h include

%.o : %.c
	$(CC)  $(CFLAGS) -c $^ -o bin/$@ $(LIBS)

server : reseaux.o server.o data.o
	$(CC)  $(CFLAGS) $^ -o bin/$@ $(LIBS)

client : client.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o bin/$@ $(LIBS)

send : send.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o bin/$@ $(LIBS)

receive : receive.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o bin/$@ $(LIBS)

zender : zender.o
	$(CC)  $(CFLAGS) $^ -o bin/$@ $(LIBS)

clean :
	rm bin/*
