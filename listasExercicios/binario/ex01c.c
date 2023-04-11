#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    FILE *arq;
    int i, ret, qtd, menor, maior, aux, j;
    long *value;

    srand(time(NULL));
    if (! (arq = fopen("numeros.bin", "r+"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    qtd = 10000;
    if (! (value = calloc(qtd, sizeof(long))))
        return 1;

    ret = fread(value, sizeof(long), qtd, arq);

    for (i = 0; i < ret; i++)
        printf("%ld ", value[i]);
    printf("\n");

//    printf("Ultimos 10 valores:\n");
//    for (i = 0; i < 10; i++)
//        printf("%ld ", value[ret - i - 1]);
//    printf("\n");

    fclose(arq);
    free(value);
    return 0;
}


