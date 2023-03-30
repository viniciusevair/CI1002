#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int palindromo (char *s, int tam) {
    int i, j, palindromoFlag;

    palindromoFlag = 1;
    j = strlen(s) - 1;
    i = 0;
    while (i < tam/2 && palindromoFlag) {
        if (s[i] == ' ')
            i++;
        if (s[j] == ' ')
            j--;

        if (s[i] != s[j])
            palindromoFlag = 0;
        i++;
        j--;
    }

    return palindromoFlag;
}

int main () {
    char *s;
    int i;

    if (! (s = calloc(100, sizeof(char))))
        return 1;

    fgets(s, 100, stdin);
    s[strcspn (s, "\n")] = 0;

    i = 0;

    if (palindromo(s, strlen(s)))
        printf("É palindromo.\n");
    else
        printf("Não é palindromo.\n");;

    free(s);

    return 0;
}

