CFLAGS = -Wall -g
LDFLAGS = -lm
PROGRAM_NAME = vina++
SRC_DIR = src

CC = gcc

# arquivos-objeto
OBJECTS = $(SRC_DIR)/main.o $(SRC_DIR)/lib_arquivos.o 
OBJECTS += $(SRC_DIR)/lib_lista.o $(SRC_DIR)/lib_vina.o

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECTS)
	$(CC) -o $(PROGRAM_NAME) $(OBJECTS) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(PROGRAM_NAME)
