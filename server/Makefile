CFLAGS = -c -Wall
CC = gcc

all: server

server: server.o fields.o
	${CC} server.o fields.o -o server

server.o: server.c
	${CC} ${CFLAGS} server.c
fields.o: fields.c
	${CC} ${CFLAGS} fields.c

clean:
	rm -f *.o *~ 

