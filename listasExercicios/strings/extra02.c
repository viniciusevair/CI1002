/*
 * Unfinished.
 * Vou voltar aqui quando souber implementar árvores binárias.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void listaPalavras (char *s) {
    int i, count;

    count = 0;

    for (i = 0; i < strlen(s); i++)
        if (s[i] == ' ' || s[i] == '\n') {
            count++;
        }

    printf("\n");
}

int main () {
    char *s;
    char caractere;
    int i, tam;

    tam = 10000;
    if (! (s = calloc(tam, sizeof(char))))
        return 1;

    printf("Digite pelo menos 20 palavras (separadas por espaço):\n");
    fgets(s, 10000, stdin);
    s[strcspn (s, "\n")] = 0;

    listaPalavras (s);

    free(s);

    return 0;
}