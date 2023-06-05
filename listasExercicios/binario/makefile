# Makefile generico

CFLAGS = -Wall -std=c99 -g  # flags de compilacao
LDFLAGS = -lm
programName = poema
programLib = libPoema

CC = gcc

# arquivos-objeto
objects = $(programName).o $(programLib).o

$(programName): $(objects)
	$(CC) -o $(programName) $(objects) $(LDFLAGS)

$(programLib).o: $(programLib).c
	$(CC) -c $(CFLAGS) $(programLib).c

$(programName).o: $(programName).c
	$(CC) -c $(CFLAGS) $(programName).c

clean:
	rm -f $(objects)

purge: clean
	rm -f $(programName)

run: $(programName)
	./$(programName)
