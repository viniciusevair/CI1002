#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    FILE *arq;
    int i, ret, qtd;
    long *value;

    srand(time(NULL));
    if (! (arq = fopen(argv[1], "a"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    qtd = rand() % (100000 - 100 + 1) + 100;
    if (! (value = calloc(qtd, sizeof(long))))
        return 1;

    for (i = 0; i < qtd; i++)
        value[i] = (rand() << 4) + (rand());

    ret = fwrite(value, sizeof(long), qtd, arq);

    if (ret)
        printf("Gravou %d valores!\n", ret);
    else
        printf("Erro ao gravar!\n");

    fclose(arq);
    free(value);
    return 0;
}
