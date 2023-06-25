#include "lib_arquivos.h"
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

/*
 * Cria um arquivo em modo de escrita e leitura para ser manipulado. O caso de
 * falha impossibilita a continuação do programa e portanto finaliza a execução
 * com valor EXIT_FAILURE.
 */
FILE* make_archiver(FILE *archive, char *filename) {
    archive = fopen(filename, "wb+");
    if(! archive) {
        fprintf(stderr, "Erro ao abrir/criar arquivador.");
        exit(EXIT_FAILURE);
    }

    return archive;
}

/*
 * Abre um arquivo em modo de leitura e escrita para ser manipulado. O caso de
 * falha ao abrir o arquivo indica que ele ainda não existe e portanto, a função
 * tenta criá-lo.
 */
FILE* open_archiver(char *filename) {
    FILE *archive;

    archive = fopen(filename, "rb+");
    if(archive == NULL)
        archive = make_archiver(archive, filename);

    return archive;
}

/*
 * Verifica se o filename é apenas o nome de um arquivo no diretório corrente ou
 * se possui algum diretório em sua especificação também.
 */
int single_name(char *filename) {
    int count = 0;
    for(int i = 0; i < strlen(filename); i++) {
        if(filename[i] == '/')
            count++;
        if(count > 1) // count > 1 implica que há mais de uma "/" no filename e
            return 0; // portanto, possui um diretório no nome.
    }

    return 1;
}

/*
 * Corrige os caminhos de arquivo para que todos tenham como endereço raíz o
 * diretório corrente. Por uma questão puramente estética, arquivos cujo caminho
 * final são o próprio diretório corrente e que foram passados de maneira não
 * absoluta (por ex.: ./arquivo_pequeno) são representados apenas com o seu
 * nome, sem indicador de caminho (por ex.: arquivo_pequeno).
 */
char *relativize_filepath(char *filename) {
    char *new_filename = strdup(filename);
    char new_filepath[FILENAME_MAX] = ".";
    char current_directory[3] = "./";
    char parent_directory[4] = "../";

    char *parent_dir_ptr = strstr(new_filename, parent_directory);

    /*
     * Verifica se o arquivo começa com um indicador de diretório pai. Em caso
     * positivo, transforma o indicador de diretório pai num indicador de
     * diretório corrente.
     */
    if(parent_dir_ptr != NULL && strncmp(parent_dir_ptr, new_filename, 4) == 0)
        memmove(new_filename, new_filename + 1, strlen(new_filename));

    char *current_dir_ptr = strstr(new_filename, current_directory);

    /*
     * Verifica se o arquivo começa com um indicador de diretório corrente, para
     * remover o indicador no caso de arquivo sem diretórios.
     */
    if(current_dir_ptr != NULL && strncmp(current_dir_ptr, new_filename, 3) == 0) {
        if(single_name(new_filename))
            strcpy(new_filename, strrchr(new_filename, '/') + 1);
        return new_filename;
    }

    // Adiciona a "/" ao inicio caso o filename não comece com este caractere.
    if(new_filename[0] != '/')
        strcat(new_filepath, "/");

    // Por fim, adiciona o "." no início do nome para garantir que todo filename
    // passado esteja referente ao diretório corrente.
    if(strrchr(new_filename, '/')) {        
        strcat(new_filepath, new_filename); 
        strcpy(new_filename, new_filepath); 
    }

    return new_filename;
}

/*
 * Cria os diretórios indicados no filename caso estes ainda não existam.
 * Retorna 1 em caso de sucesso e 0 em caso de erros.
 */
int make_directories(char *filename, mode_t mode) {
    char *modifiable_path = strdup(filename);
    char filepath[FILENAME_MAX] = ".";
    char *token = strtok(modifiable_path, "/");

    while(token != NULL) {
        strcat(filepath, "/");
        strcat(filepath, token);
        if(mkdir(filepath, mode) != 0) {
            if(errno == EEXIST) {           // O erro EEXIST apenas indica que o
                token = strtok(NULL, "/");  // subdiretório já existe e portanto
                continue;                   // o laço pode seguir para a próxima
            }                               // iteração. 

            return 0;
        }

        token = strtok(NULL, "/");
    }

    return 1;
}

// Verifica se há a necessidade de gerenciar diretórios no filename.
void verify_directories(char *filename) {
    char *path_separator = strrchr(filename, '/');
    if(path_separator != NULL) {
        *path_separator = 0;
        if(! make_directories(filename, S_IRWXU))
            fprintf(stderr, "Erro ao criar diretório %s\n", filename);
        *path_separator = '/';
    }
}

/*
 * Cria um arquivo em disco com o nome filename e o abre para escrita.
 */
FILE *make_member(char *filename) {
    FILE *member;

    verify_directories(filename);
    member = fopen(filename, "wb");
    if(member == NULL) {
        fprintf(stderr, "Erro ao criar arquivo %s\n", filename);
    }

    return member;
}

// Abre o arquivo filename em modo de leitura.
FILE* open_member(char *filename) {
    FILE *member;

    member = fopen(filename, "r");
    if(member == NULL)
        fprintf(stderr, "Erro ao abrir arquivo %s\n", filename);

    return member;
}

/*
 * Recebe como parâmetro o nome de um arquivo.
 * Guarda os metadados do arquivo em uma struct. A responsabilidade por
 * desalocar a memória é de quem chamou a função.
 * Retorna a struct contendo os metadados.
 */
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

// Retorna o caractere referente ao tipo especificado pelo valor de mode.
char type_handler(int mode) {
    switch(mode & S_IFMT) {
        case S_IFBLK:       // Block device
            return 'b';
        case S_IFCHR:       // Character device
            return 'c';
        case S_IFDIR:       // Directory
            return 'd';
        case S_IFIFO:       // FIFO file
            return 'p';
        case S_IFLNK:       // Symbolic link
            return 'l';
        case S_IFREG:       // Regular file
            return '-';
        case S_IFSOCK:      // Socket
            return 's';
        default:
            return '-';
    }
}

// Imprime o tipo de arquivo e as permissões para usuário, grupo e outros.
void print_permissions(mode_t mode) {
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

// Imprime a data especificada por time no formato americano YYYY-MM-DD H:M
void print_time(time_t time) {
    struct tm *formated_time = localtime(&time);
    char date[200];

    strftime(date, sizeof(date), "%F %H:%M", formated_time);
    printf("%s", date);
}

// Imprime todos os metadados do arquivo. I. e., os dados guardados na struct.
void print_file_data(struct file_header_t *file) {
    char *user_name, *group_name;
    user_name = getpwuid(file->user_id)->pw_name;
    group_name = getgrgid(file->group_id)->gr_name;

    print_permissions(file->mode);
    printf(" %s/%s %zu ", user_name, group_name, file->size);
    print_time(file->modif_date);
    printf(" %s\n", file->filename);
}
