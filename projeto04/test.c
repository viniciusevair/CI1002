#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "libArquivos.h"

int main(int argc, char *argv[]) {
    FILE *arq;
    struct file_header_t *teste1, *teste2;
    if(! (teste1 = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    if(! (teste2 = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    arq = abre_arquivador(argv[1]);
    teste1 = pega_dados(argv[2]);
    fwrite(teste1, sizeof(struct file_header_t), 1, arq);
    free(teste1);

    rewind(arq);
    fread(teste2, sizeof(struct file_header_t), 1, arq);
    printf("TESTE:\n");
    printf("Filename: %s\nModif date: %zu\n", teste2->filename, teste2->modif_date);
    printf("MAIS TESTES:\n");
    printf("Filepath: %s\nUID: %zu\n", teste2->filepath, teste2->user_id);

    return 0;
}
