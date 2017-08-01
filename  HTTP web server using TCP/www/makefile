C = gcc
CFLAGS = -Wall -O3
LDFLAGS = -L$(MACPORTS)/lib
OBJECTS = web_server.o

web_server : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o web_server

web_server.o : web_server.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c web_server.c

.PHONY : clean
clean :
	rm web_server.o 

