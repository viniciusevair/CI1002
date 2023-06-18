#ifndef __ARQUIVOS
#define __ARQUIVOS

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NAME_MAX 255

struct file_header_t {
    char filename[NAME_MAX];
    size_t order;
    char filepath[FILENAME_MAX];
    uid_t user_id;
    gid_t group_id;
    mode_t mode;
    size_t size;
    time_t modif_date;
    size_t archive_position;
};

FILE* open_archiver(char *filename);
FILE* open_member(char *filename);
struct file_header_t* get_data(char *filename);
void write_file_data(struct file_header_t *file);

#endif
