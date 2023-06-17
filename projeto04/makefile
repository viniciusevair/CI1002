CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++

CC = gcc

# arquivos-objeto
OBJECTS = libArquivos.o libLista.o

all: $(PROGRAM_NAME)

test: test.o libArquivos.o libLista.o
	$(CC) -o teste test.o $(OBJECTS) $(LDFLAGS)

test.o: test.c
	$(CC) -c $(CFLAGS) test.c

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) main.o $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

libArquivos.o: libArquivos.c
	$(CC) -c $(CFLAGS) libArquivos.c

libLista.o: libLista.c
	$(CC) -c $(CFLAGS) libLista.c

clean:
	rm -f $(OBJECTS)

purge: clean
	rm -f $(PROGRAM_NAME)

