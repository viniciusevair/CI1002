#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

/* 
 * Busca a quantidade de bytes no arquivo e divide pelo tamanho de long.
 * Basicamente, retorna a quantidade de números escritos no ex01a.
 */
long tamanhoArquivo() {
    struct stat st;
    stat("numeros.bin", &st);

    return st.st_size / sizeof(long);
}

int main(int argc, char *argv[]) {
    FILE *arq;
    int i, ret, qtd, menor, maior, aux, j;
    long *value;

    srand(time(NULL));
    if (! (arq = fopen("numeros.bin", "r"))) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    qtd = tamanhoArquivo();
    if (! (value = calloc(qtd, sizeof(long))))
        return 1;

    ret = fread(value, sizeof(long), qtd, arq);

    printf("Primeiros 10 valores:\n");
    for (i = 0; i < 10; i++)
        printf("%ld ", value[i]);
    printf("\n");

    printf("Ultimos 10 valores:\n");
    for (i = ret - 11; i < ret; i++)
        printf("%ld ", value[i]);
    printf("\n");

    fclose(arq);
    free(value);
    return 0;
}
