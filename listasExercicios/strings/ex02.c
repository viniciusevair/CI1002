#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char *stringGenerica;
    int i;

    if (! (stringGenerica = calloc(100, sizeof(char))))
        return 1;

    fgets(stringGenerica, 100, stdin);
    stringGenerica[strcspn (stringGenerica, "\n")] = 0;

    for (i = strlen(stringGenerica); i >= 0; i--)
        printf("%c", stringGenerica[i]);
    printf("\n");

    free(stringGenerica);

    return 0;
}

