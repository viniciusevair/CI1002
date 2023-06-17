#ifndef __ARQUIVOS
#define __ARQUIVOS

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NAME_MAX 255
#define PATH_MAX 4096

struct file_header_t {
    char filename[NAME_MAX];
    size_t order;
    char filepath[PATH_MAX];
    uid_t user_id;
    gid_t group_id;
    mode_t permissions;
    size_t size;
    time_t modif_date;
    size_t archive_position;
};

FILE* abre_arquivador(char *filename);
FILE* abre_membro(char *filename);
struct file_header_t* pega_dados(char *filename);
void escreve_info_arquivo(struct file_header_t *file);

#endif
