CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++

CC = gcc

# arquivos-objeto
OBJECTS = main.o

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f $(OBJECTS)

purge: clean
	rm -f $(PROGRAM_NAME)

