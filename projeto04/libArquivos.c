#include "libArquivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
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

int single_name(char *filename) {
    int count = 0;
    for(int i = 0; i < strlen(filename); i++) {
        if(filename[i] == '/')
            count++;
        if(count > 1)
            return 0;
    }

    return 1;
}

char *relativize_filepath(char *filename) {
    char *new_filename = strdup(filename);
    char new_filepath[FILENAME_MAX] = ".";
    char current_directory[3] = "./";
    char parent_directory[4] = "../";
    char home_directory[3] = "~/";

    char *current_dir_ptr = strstr(new_filename, current_directory);
    char *parent_dir_ptr = strstr(new_filename, parent_directory);
    char *home_dir_ptr = strstr(new_filename, home_directory);

    // Verifica se o arquivo começa com um indicador de diretório corrente.
    if(current_dir_ptr != NULL && strncmp(current_dir_ptr, new_filename, 3) == 0) {
        if(single_name(new_filename))
            strcpy(new_filename, strrchr(new_filename, '/') + 1);
        return new_filename;
    }
    // Verifica se o arquivo começa com um indicador de diretório pai.
    if(parent_dir_ptr != NULL && strncmp(parent_dir_ptr, new_filename, 4) == 0)
        return new_filename;
    // Verifica se o arquivo começa com um indicador de diretório home.
    if(home_dir_ptr != NULL && strncmp(home_dir_ptr, new_filename, 3) == 0)
        return new_filename;

    /*
     * Verifica se o nome do arquivo contém um caminho absoluto ou diretório
     * corrente, mas sem indicador no inicio.
     */
    if(new_filename[0] != '/')
        strcat(new_filepath, "/");

    if(strrchr(new_filename, '/')) {        
        strcat(new_filepath, new_filename); 
        strcpy(new_filename, new_filepath); 
    }

    return new_filename;
}

int make_directories(char *filename, mode_t mode) {
    char *modifiable_path = strdup(filename);
    char filepath[FILENAME_MAX] = ".";
    char *token = strtok(modifiable_path, "/");

    while (token != NULL) {
        strcat(filepath, "/");
        strcat(filepath, token);
        if(mkdir(filepath, mode) != 0) {
            if(errno == EEXIST) {
                token = strtok(NULL, "/");
                continue;
            }
            return 0;
        }

        token = strtok(NULL, "/");
    }

    return 1;
}

void verify_directories(char *filename) {
    char *path_separator = strrchr(filename, '/');
    if(path_separator != NULL) {
        *path_separator = 0;
        if(! make_directories(filename, S_IRWXU))
            fprintf(stderr, "Erro ao criar diretório %s\n", filename);
        *path_separator = '/';
    }
}

FILE *make_member(char *filename) {
    FILE *member;

    verify_directories(filename);
    member = fopen(filename, "wb");
    if(member == NULL) {
        member = fopen(filename, "wb");
        if(member == NULL)
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
    char *user_name, *group_name;
    user_name = getpwuid(file->user_id)->pw_name;
    group_name = getgrgid(file->group_id)->gr_name;

    write_permissions(file->mode);
    printf(" %s/%s %zu ", user_name, group_name, file->size);
    write_time(file->modif_date);
    printf(" %s\n", file->filename);
}
