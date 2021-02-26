CC = gcc
GTKCFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic -pipe -pthread
GTKLIBS = `pkg-config --libs gtk+-3.0`

CFLAGS = -g -pthread $(GTKCFLAGS)
LIBS = $(GTKLIBS)

all : server client send receive zender

server : reseaux.o server.o data.o
	$(CC)  $(CFLAGS) $^ -o $@ $(LIBS)

client : client.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o $@ $(LIBS)

send : send.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o $@ $(LIBS)

receive : receive.o reseaux.o data.o
	$(CC)  $(CFLAGS) $^ -o $@ $(LIBS)

zender : data.o reseaux.o zender.o
	$(CC)  $(CFLAGS) $^ -o $@ $(LIBS)

clean :
	rm *.o

properClean : clean
	rm server  client  send  receive  zender