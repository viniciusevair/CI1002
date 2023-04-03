#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void troca (char *s, int a, int b) {
    int aux;

    aux = s[a];
    s[a] = s[b];
    s[b] = aux;
}

void adicionaColchetes (char *s, int nonAlpha, int tam) {
    int i;

    s[tam+1] = '[';
    for (i = tam; i >= nonAlpha; i--)
        troca(s, i, i+1);

    s[tam+2] = ']';
    for (i = tam+1; i > nonAlpha + 1; i--)
        troca(s, i, i+1);
}

int destacaCaractere (char *s, int tam) {
    int i;

    i = 0;
    while(i < tam) {
        if ((s[i] < 'a' || s[i] > 'z') && (s[i] < 'A' || s[i] > 'Z') &&
                (s[i] < '0' || s[i] > '9') && (s[i] != ' ')) {
            adicionaColchetes(s, i, tam);
            i+= 2;
            tam += 2;
        }

        i++;
    }

    return 1;
}

int main () {
    char *string;
    int i;

    if (! (string = calloc(10000, sizeof(char))))
        return 1;

    fgets(string, 10000, stdin);
    string[strcspn (string, "\n")] = 0;

    destacaCaractere(string, strlen(string));
    printf("%s\n", string);

    free(string);

    return 0;
}
