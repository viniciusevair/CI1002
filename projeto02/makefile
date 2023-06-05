CFLAGS = -Wall -std=c99 -g
LDFLAGS = -lm
programName = beale

CC = gcc

# arquivos-objeto
objects = $(programName).o libAVL.o libSubAVL.o libCodifica.o libDecodifica.o libCifra.o libTratamento.o

all: $(programName)

$(programName): $(objects)
	$(CC) -o $(programName) $(objects) $(LDFLAGS)

libSubAVL.o: libSubAVL.c
	$(CC) -c $(CFLAGS) libSubAVL.c

libAVL.o: libAVL.c
	$(CC) -c $(CFLAGS) libAVL.c

libCodifica.o: libCodifica.c
	$(CC) -c $(CFLAGS) libCodifica.c

libDecodifica.o: libDecodifica.c
	$(CC) -c $(CFLAGS) libDecodifica.c

libCifra.o: libCifra.c
	$(CC) -c $(CFLAGS) libCifra.c

libTratamento.o: libTratamento.c
	$(CC) -c $(CFLAGS) libTratamento.c

$(programName).o: $(programName).c
	$(CC) -c $(CFLAGS) $(programName).c

clean:
	rm -f $(objects)

purge: clean
	rm -f $(programName)
