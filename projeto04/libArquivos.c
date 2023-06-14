#include "libArquivos.h"
#include <stdio.h>
#include <stdlib.h>

FILE* cria_arquivador(FILE *archive, char *filename) {
    archive = fopen(filename, "w+");
    if(! archive) {
        fprintf(stderr, "Erro ao criar arquivo.");
        exit(EXIT_FAILURE);
    }

    return archive;
}

FILE* abre_arquivador(char *filename) {
    FILE *archive;

    archive = fopen(filename, "r+");
    if(archive == NULL)
        archive = cria_arquivador(archive, filename);

    return archive;
}

FILE* abre_membro(char *filename) {
    FILE *membro;

    membro = fopen(filename, "r");
    if(membro == NULL)
        fprintf(stderr, "Erro ao abrir arquivo %s.", filename);

    return membro;
}

//TODO:
/// Pegar dados dos arquivos;
