#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encontraPalavras (char *s, char caractere) {
    int i, count;

    count = 0;
    if (s[0] == caractere)
        printf("%d ", count);    

    for (i = 0; i < strlen(s); i++)
        if (s[i] == ' ') {
            count++;
            if (s[i + 1] == caractere)
                printf("%d ", count);
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

    printf("Digite uma letra ou número:\n");
    scanf("%c", &caractere);

    encontraPalavras (s, caractere);

    free(s);

    return 0;
}
