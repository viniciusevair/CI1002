#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

long tamanhoPoema() {
    struct stat st;
    stat("poema.bin", &st);

    return st.st_size / 5;
}

void organizaPoema(FILE *arq, char *poema) {
    int pos;
    char letra;

    fread(&pos, sizeof(int), 1, arq);
    fread(&letra, sizeof(char), 1, arq);
    while (! feof(arq)) {
        poema[pos] = letra;

        fread(&pos, sizeof(int), 1, arq);
        fread(&letra, sizeof(char), 1, arq);
    }
}

int main(int argc, char *argv[]) {
    FILE *arq;
    char *poema;

    if (! (arq = fopen("poema.bin", "r+"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    if (! (poema = calloc(tamanhoPoema(), sizeof(char))))
        return 1;

    organizaPoema(arq, poema);

    printf("%s\n", poema);

    free(poema);
    fclose(arq);
    return 0;
}
