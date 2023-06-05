CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAMNAME = rle

CC = gcc

all: $(PROGRAMNAME)

$(PROGRAMNAME): main.o
	$(CC) -o $(PROGRAMNAME) main.o $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

run: all
	./$(PROGRAMNAME)

clean:
	rm -f main.o $(PROGRAMNAME)
