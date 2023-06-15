#ifndef __ARQUIVOS
#define __ARQUIVOS

#include <stdio.h>
#include <sys/stat.h>

#define NAME_MAX 255
#define PATH_MAX 4096

struct file_header_t {
    char filename[NAME_MAX];
    char filepath[PATH_MAX];
    size_t user_id;
    size_t group_id;
    size_t permissions;
    size_t size;
    size_t modif_date;
    unsigned int archive_position;
};

FILE* abre_arquivador(char *filename);
FILE* abre_membro(char *filename);
struct file_header_t* pega_dados(char *filename);

#endif
