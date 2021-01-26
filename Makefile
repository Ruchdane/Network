CC = gcc
CFLAGS = -g -pthread

all : server client send receive

server : reseaux.o server.o data.o
	$(CC) $(CFLAGS) reseaux.o server.o data.o -o server

client : client.o reseaux.o data.o
	$(CC) $(CFLAGS) client.o reseaux.o data.o -o client

send : send.o server.o data.o
	$(CC) $(CFLAGS) send.o reseaux.o data.o -o send

receive : receive.o reseaux.o data.o
	$(CC) $(CFLAGS) receive.o reseaux.o data.o -o receive


clean :
	rm *.o

properClean : clean
	rm server
	rm client
