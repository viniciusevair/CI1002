CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++

CC = gcc

# arquivos-objeto
OBJECTS = main.o lib_arquivos.o lib_lista.o lib_vina.o

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) $(OBJECTS) $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

lib_arquivos.o: lib_arquivos.c
	$(CC) -c $(CFLAGS) lib_arquivos.c

lib_lista.o: lib_lista.c
	$(CC) -c $(CFLAGS) lib_lista.c

lib_vina.o: lib_vina.c
	$(CC) -c $(CFLAGS) lib_vina.c

clean:
	rm -f $(OBJECTS)

purge: clean
	rm -f $(PROGRAM_NAME)

