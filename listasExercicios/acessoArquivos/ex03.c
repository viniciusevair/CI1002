#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINEMAX 1024

int main () {
    FILE *srce, *dest;
    char *s;

    if (! (s = calloc(LINEMAX, sizeof(char)))) {
        perror ("Erro ao alocar memória.");
        exit(1);
    }
    if (! (srce = fopen("minusc.txt", "r"))) {
        perror("Erro ao abrir o arquivo.");
        exit (1);
    }
    if (! (dest = fopen("maiusc.txt", "w"))) {
        perror("Erro ao criar o arquivo.");
        exit (1);
    }

    fgets(s, LINEMAX, srce);
    while (! feof(srce)) {
        for (int i = 0; i < strlen(s); i++)
            if (s[i] >= 'a' && s[i] <= 'z')
                s[i] -= 32;

        fprintf(dest, "%s", s);
        fgets(s, LINEMAX, srce);
    }

    free(s);
    fclose(srce);
    return 0;
}

