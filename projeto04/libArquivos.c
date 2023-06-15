#include "libArquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


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

struct file_header_t* pega_dados(char *filename) {
    struct file_header_t *file_data;
    char *aux;
    struct stat *data_buffer;

    if(! (file_data = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória");
        return NULL;
    }
    if(! (data_buffer = malloc(sizeof(struct stat)))) {
        fprintf(stderr, "Erro ao alocar memória");
        return NULL;
    }

    stat(filename, data_buffer);
    aux = strrchr(filename, '/');
    if(! aux)
        aux = strdup(filename);
    else
        aux = strdup(aux + 1);

    strcpy(file_data->filename, aux);
    file_data->modif_date = 8;
    file_data->permissions = 1;
    file_data->group_id = 7;
    file_data->archive_position = 90;
    file_data->user_id = 666;
    file_data->size = 30;
    strcpy(file_data->filepath, "oi");

    free(data_buffer);
    return file_data;
}

//TODO:
/// Pegar dados dos arquivos;
