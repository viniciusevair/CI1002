CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++

CC = gcc

# arquivos-objeto
OBJECTS = main.o libArquivos.o libLista.o libVina.o

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) $(OBJECTS) $(LDFLAGS)

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

