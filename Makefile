all :server client send receive zender
CC = gcc
CFLAGS = -ggdb   $(GTKCFLAGS)
LIBS = -Iinclude $(GTKLIBS) $(NETWORKLIBS)

GTKCFLAGS = `pkg-config --cflags gtk+-3.0` -pipe 
GTKLIBS = ` pkg-config --libs gtk+-3.0`
ifeq ($(OS),Windows_NT)
CFLAGS += -llibwinpthread
NETWORKLIBS = -lws2_32 -liphlpapi
else
GTKCFLAGS += -rdynamic 
NETWORKLIBS = 
endif


vpath %.c src
vpath %.o bin
vpath %.h include

%.o : %.c
	$(CC)  $(CFLAGS) -c $^ -o bin/$@ $(LIBS)

server : reseaux.o server.o data.o
	$(CC)  $(CFLAGS) bin/reseaux.o bin/server.o bin/data.o -o bin/$@ $(LIBS)

client : client.o reseaux.o data.o
	$(CC)  $(CFLAGS)  bin/client.o bin/reseaux.o bin/data.o -o bin/$@ $(LIBS)

send : send.o reseaux.o data.o
	$(CC)  $(CFLAGS) bin/send.o bin/reseaux.o bin/data.o -o bin/$@ $(LIBS)

receive : receive.o reseaux.o data.o
	$(CC)  $(CFLAGS) bin/receive.o bin/reseaux.o bin/data.o -o bin/$@ $(LIBS)

zender : zender.o
	$(CC)  $(CFLAGS) bin/zender.o -o bin/$@ $(LIBS)

clean :
	rm bin/*.o
