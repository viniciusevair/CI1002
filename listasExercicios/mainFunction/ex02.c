#include <stdlib.h>
#include <stdio.h>
#define LINEMAX 1024

int main(int argc, char *argv[]) {
    FILE *arq;
    char *s;

    if (! (s = calloc(LINEMAX, sizeof(char)))) {
        perror ("Erro ao alocar memória.");
        exit(1);
    }

    if (! (arq = fopen(argv[1], "r"))) {
        free(s);
        return 1;
    }

    free(s);
    return 0;
}
