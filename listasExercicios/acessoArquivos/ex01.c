#include <stdio.h>
#include <stdlib.h>

int contaCaractere (FILE *arq) {
    int count;

    count = 0;
    while (getc(arq) != EOF)
        count++;
    count--;

    return count;
}

int main () {
    FILE *arq;
    int count;

    if (! (arq = fopen("arqTexto.txt", "r"))) {
        perror("Erro ao abrir o arquivo.");
        exit (1);
    }

    printf("A quantidade de caracteres é %d\n", contaCaractere(arq));

    fclose(arq);
    return 0;
}
