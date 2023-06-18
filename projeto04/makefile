CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++

CC = gcc

# arquivos-objeto
OBJECTS = libArquivos.o libLista.o libVina.o

all: $(PROGRAM_NAME)

test: test.o $(OBJECTS)
	$(CC) -o teste test.o $(OBJECTS) $(LDFLAGS)

test.o: test.c
	$(CC) -c $(CFLAGS) test.c

$(PROGRAM_NAME): main.o $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) main.o $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

libArquivos.o: libArquivos.c
	$(CC) -c $(CFLAGS) libArquivos.c

libLista.o: libLista.c
	$(CC) -c $(CFLAGS) libLista.c

libVina.o: libVina.c
	$(CC) -c $(CFLAGS) libVina.c

clean:
	rm -f $(OBJECTS)

purge: clean
	rm -f $(PROGRAM_NAME)

