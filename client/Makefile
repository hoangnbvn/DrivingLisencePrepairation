CFLAGS = -c -Wall
CC = gcc

all: main

main: main.c fields.c
	${CC} main.c fields.c message.c menu.c luyentap.c lambai.c vaothi.c roomlist.c createroom.c showresultsofroom.c -o main `pkg-config --cflags --libs gtk+-3.0` 
clean:
	rm -f *.o *~ 

