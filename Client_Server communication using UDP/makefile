C = gcc
CFLAGS = -Wall -O3
LDFLAGS = -L$(MACPORTS)/lib
OBJECTS = client.o
OBJECTS1 = server.o

all: client server

client : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o client

client.o : client.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c client.c

server: $(OBJECTS1)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS1) -o server

server.o : server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c server.c

.PHONY : clean
clean :
	rm server.o client.o

