#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    char *stringGenerica;
    int i, count;

    if (! (stringGenerica = calloc(100, sizeof(char))))
        return 1;

    fgets(stringGenerica, 100, stdin);
    stringGenerica[strcspn (stringGenerica, "\n")] = 0;

    i = 0;
    while (stringGenerica[i])
        i++;

    printf("Contando com um for: %d\n", i);
    printf("Contando com strlen: %lu\n", strlen(stringGenerica));

    free(stringGenerica);

    return 0;
}

