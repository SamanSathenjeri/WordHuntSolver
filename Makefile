OBJS	= dictionary.o wordHunt.o Node.o
SOURCE	= dictionary.c wordHunt.c Node.c
HEADER	= dictionary.h wordHunt.h Node.h
OUT	= wordHunt
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

dictionary.o: dictionary.c
	$(CC) $(FLAGS) dictionary.c 

wordHunt.o: wordHunt.c
	$(CC) $(FLAGS) wordHunt.c 

Node.o: Node.c
	$(CC) $(FLAGS) Node.c 

clean:
	rm -f $(OBJS) $(OUT)