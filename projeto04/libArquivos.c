#include "libArquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


FILE* make_archiver(FILE *archive, char *filename) {
    archive = fopen(filename, "wb+");
    if(! archive) {
        fprintf(stderr, "Erro ao criar arquivo.");
        exit(EXIT_FAILURE);
    }

    return archive;
}

FILE* open_archiver(char *filename) {
    FILE *archive;

    archive = fopen(filename, "rb+");
    if(archive == NULL)
        archive = make_archiver(archive, filename);

    return archive;
}

FILE *make_member(char *filename) {
    FILE *member;

    member = fopen(filename, "wb");
    if(member == NULL) {
        fprintf(stderr, "Erro ao criar arquivo %s\n", filename);
    }

    return member;
}

FILE* open_member(char *filename) {
    FILE *member;

    member = fopen(filename, "r");
    if(member == NULL)
        fprintf(stderr, "Erro ao abrir arquivo %s\n", filename);

    return member;
}

struct file_header_t* get_data(char *filename) {
    struct file_header_t *file_data;
    struct stat data_buffer;

    if(! (file_data = malloc(sizeof(struct file_header_t)))) {
        fprintf(stderr, "Erro ao alocar memória");
        return NULL;
    }

    stat(filename, &data_buffer);
    strcpy(file_data->filename, filename);

    file_data->modif_date = data_buffer.st_mtim.tv_sec;
    file_data->mode = data_buffer.st_mode;
    file_data->group_id = data_buffer.st_gid;
    file_data->user_id = data_buffer.st_uid;
    file_data->size = data_buffer.st_size;

    return file_data;
}

char type_handler(int mode) {
    switch(mode & S_IFMT) {
        case S_IFBLK:
            return 'b';
        case S_IFCHR:
            return 'c';
        case S_IFDIR:
            return 'd';
        case S_IFIFO:
            return 'p';
        case S_IFLNK:
            return 'l';
        case S_IFREG:
            return '-';
        case S_IFSOCK:
            return 's';
        default:
            return '-';
    }
}

void write_permissions(mode_t mode) {
    printf("%c", type_handler(mode));
    printf((mode & S_IRUSR)? "r" : "-");
    printf((mode & S_IWUSR)? "w" : "-");
    printf((mode & S_IXUSR)? "x" : "-");
    printf((mode & S_IRGRP)? "r" : "-");
    printf((mode & S_IWGRP)? "w" : "-");
    printf((mode & S_IXGRP)? "x" : "-");
    printf((mode & S_IROTH)? "r" : "-");
    printf((mode & S_IWOTH)? "w" : "-");
    printf((mode & S_IXOTH)? "x" : "-");
}

void write_time(time_t time) {
    struct tm *formated_time = localtime(&time);
    char date[200];

    strftime(date, sizeof(date), "%F %H:%M", formated_time);
    printf("%s", date);
}

void write_file_data(struct file_header_t *file) {
    write_permissions(file->mode);
    printf(" ");
    printf("%s/%s ", getpwuid(file->user_id)->pw_name, getgrgid(file->group_id)->gr_name);
    printf("%zu", file->size);
    printf(" ");
    write_time(file->modif_date);
    printf(" ");
    printf("%s\n", file->filename);
}
