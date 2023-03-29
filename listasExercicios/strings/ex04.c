#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trocar(char **s, int a, int b) {
    char *aux;

    aux = s[a];
    s[a] = s[b];
    s[b] = aux;
}

int particionar(char **s, int ini, int fim) {
    int indiceDivisor, i;

    indiceDivisor = ini;
    for(i = ini; i < fim; i++) {
        if(strcmp(s[i], s[fim]) < 0) {
            trocar(s, indiceDivisor, i);
            indiceDivisor++;
        }
    }

    trocar(s, indiceDivisor, fim);
    return indiceDivisor;        
}

void quickSort(char **s, int ini, int fim) {
    int indiceDivisor;
    int i;

    if(ini >= fim)
        return;

    indiceDivisor = particionar(s, ini, fim);
    quickSort(s, ini, indiceDivisor - 1);
    quickSort(s, indiceDivisor + 1, fim);
}

int main () {
    char **string;
    int i, j, k;

    /*
     * Como é só um exercício, não vou trabalhar com casos de extrapolar a
     * quantidade de linhas ou a quantidade de caracteres. Não é isso que está
     * sendo avaliado aqui, de qualquer forma.
     */
    if (! (string = calloc(100, sizeof(char*))))
        return 1;

    i = 0;
    do {
        if (! (string[i] = calloc(10000, sizeof(char))))
            return 1;

        fgets(string[i], 10000, stdin);
        string[i][strcspn(string[i], "\n")] = 0;

        i++;
    } while (i < 100 && string[i - 1][0] != 0);
    i--;

    quickSort(string, 0, i - 1);

    for (j = 0; j < i; j++) {
        printf("%s\n", string[j]);
    }

    for (i = 0; i < 100; i++)
        free(string[i]);
    free(string);

    return 0;
}
