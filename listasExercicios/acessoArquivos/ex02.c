#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINEMAX 1024

int main () {
    FILE *arq;
    char *s;
    int sum, num, aux, count;

    if (! (s = calloc(LINEMAX, sizeof(char)))) {
        perror ("Erro ao alocar memória.");
        exit(1);
    }
    if (! (arq = fopen("arqTexto.txt", "r"))) {
        perror("Erro ao abrir o arquivo.");
        exit (1);
    }

    sum = 0;
    count = 0;
    fgets(s, LINEMAX, arq);
    while (! feof(arq)) {
        num = 0;
        for (int i = 0; i < strlen(s); i++)
            if (s[i] != 10) {
                aux = (int)s[i] - 48;
                num *= 10;
                num += aux;
            }
        sum += num;
        count++;
        fgets(s, LINEMAX, arq);
    }
    printf("A média dos valores é %.4f\n", (float)sum/count);

    free(s);
    fclose(arq);
    return 0;
}
