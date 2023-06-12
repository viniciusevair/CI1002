#include "libArquivos.h"
#include <stdio.h>
#include <stdlib.h>

FILE* abre_arquivador(char *filename) {
    FILE *archive;

    archive = fopen(filename, "r+");
    if(! archive) {
        archive = fopen(filename, "w+");
        if(! archive) {
            fprintf(stderr, "Erro ao criar arquivo.");
            exit(EXIT_FAILURE);
        }
    }

    return archive;
}

FILE* abre_membro(char *filename) {
    FILE *membro;

    membro = fopen(filename, "r");
    if(! membro) {
        fprintf(stderr, "Erro ao abrir arquivo %s.", filename);
        exit(EXIT_FAILURE);
    }

    return membro;
}
