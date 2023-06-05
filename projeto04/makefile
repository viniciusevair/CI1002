CFLAGS = -Wall -g
LDFLAGS = -lm
programName = vina++

CC = gcc

# arquivos-objeto
objects = $(programName).o 

all: $(programName)

$(programName): $(objects)
	$(CC) -o $(programName) $(objects) $(LDFLAGS)

$(programName).o: $(programName).c
	$(CC) -c $(CFLAGS) $(programName).c

clean:
	rm -f $(objects)

purge: clean
	rm -f $(programName)

