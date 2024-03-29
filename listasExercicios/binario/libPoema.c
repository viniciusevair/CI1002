#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

long tamanhoPoema() {
    struct stat st;
    stat("poema.bin", &st);

    return st.st_size / 5;
}

void organizaPoema(FILE *arq, char *poema) {
    int pos;

    fread(&pos, sizeof(int), 1, arq);
    fread(&poema[pos], sizeof(char), 1, arq);
    while (! feof(arq)) {
        fread(&pos, sizeof(int), 1, arq);
        fread(&poema[pos], sizeof(char), 1, arq);
    }
}
