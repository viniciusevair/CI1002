#include <stdlib.h>
#include <stdio.h>
#include "libPoema.h"

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
